#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/stdcxx.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/THttpClient.h>
#include <thrift/transport/TSSLSocket.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSimpleFileTransport.h>
#include <thrift/transport/TSocket.h>

#include "Database.h"
#include "gen-cpp/NoteStore.h"
#include "gen-cpp/NoteStore_types.h"
#include "gen-cpp/UserStore.h"
#include "gen-cpp/UserStore_constants.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::stdcxx;
using namespace evernote::edam;
using namespace std;

/* Sandbox: https://sandbox.evernote.com/
 * Evernote International 产品环境： https://www.evernote.com/
 * 印象笔记产品环境： https://app.yinxiang.com/
 * 印象笔记沙盒环境：https://sandbox.yinxiang.com
 */

#define HOST_YINXIANG "app.yinxiang.com"
#define HOST_EVERNOTE "www.evernote.com"
#define HOST_EVERNOTE_SANDBOX "sandbox.evernote.com"
#define HOST_YINXIANG_SANDBOX "sandbox.yinxiang.com"

const string g_BaseUrl = "put your host url here";
const string g_AuthToken = "put your dev token here";

const string g_EvernoteDataLocation = "./evernote-data";

int32_t updateNotebooks(shared_ptr<EvernoteDB> &, vector<Notebook> &,
                        vector<string> &);
int32_t updateNotes(shared_ptr<EvernoteDB> &, vector<Note> &, vector<string> &);
int32_t updateTags(shared_ptr<EvernoteDB> &, vector<Tag> &, vector<string> &);
static void getServerUpdateInfo(vector<SyncChunk> &, vector<Notebook> &,
                                vector<Note> &, vector<Tag> &, vector<string> &,
                                vector<string> &, vector<string> &);

int main() {
    if (g_BaseUrl == "put your host url here") {
        cerr << "Invalid host url, exit!!!" << endl;
        return -1;
    }
    if (g_AuthToken == "put your dev token here") {
        cerr << "Invalid Token, exit!!!" << endl;
        return -1;
    }

    /* create a EvernoteDB to hold client data, that includes client-side sync
     * variables and serialized Evernote data objects. */
    shared_ptr<EvernoteDB> db(new EvernoteDB(g_EvernoteDataLocation));
    if (!db) {
        cerr << "new EvernoteDB() failed!!!" << endl;
        return -1;
    }

    /* restore client variables and Evernote data objects. */
    if (db->open() < 0) {
        cerr << "open database failed, set lastUpdateCount = 0 to perform a "
                "full sync."
             << endl;
        db->lastUpdateCount = 0;
    } else {
        cout << "open database success." << endl;
    }

    /* create SSLSocket factory, CA is needed to avoid "certificate verify
     * failed" error */
    shared_ptr<TSSLSocketFactory> sslSocketFactory(new TSSLSocketFactory());
    /* CA is copied from /etc/ssl/certs/ca-certificates.crt */
    sslSocketFactory->loadTrustedCertificates("./keys/ca-certificates.crt");
    sslSocketFactory->authenticate(true);

    /* use SSLSocket factory to create a UserStoreClient */
    shared_ptr<TSocket> userStoreSSLSocket =
        sslSocketFactory->createSocket(g_BaseUrl, 443);
    shared_ptr<TBufferedTransport> userStoreBufferedTransport(
        new TBufferedTransport(userStoreSSLSocket));
    shared_ptr<TTransport> userStoreTransport(
        new THttpClient(userStoreBufferedTransport, g_BaseUrl, "/edam/user"));
    shared_ptr<TProtocol> userStoreProtocol(
        new TBinaryProtocol(userStoreTransport));
    UserStoreClient userStore(userStoreProtocol);
    userStoreTransport->open();

    /* Check version */
    bool ok = userStore.checkVersion("Evernote C++ APP/1.0.0; Ubuntu16.04",
                                     g_UserStore_constants.EDAM_VERSION_MAJOR,
                                     g_UserStore_constants.EDAM_VERSION_MINOR);
    if (ok) {
        cout << "check version ok!" << endl;
    } else {
        cout << "check version failed!" << endl;
        return -1;
    }

    /* dump UserStore information */
    User user;
    UserUrls userUrl;
    try {
        userStore.getUser(user, g_AuthToken);
        userStore.getUserUrls(userUrl, g_AuthToken);

        cout << "username       : " << user.username << endl;
        cout << "userid         : " << user.id << endl;
        cout << "email          : " << user.email << endl;
        cout << "user url       : " << userUrl.userStoreUrl << endl;
        cout << "note url       : " << userUrl.noteStoreUrl << endl;
    } catch (TException &tx) {
        cout << "UserStore ERROR: " << tx.what() << endl;
        return -1;
    }

    /* create a NoteStoreClient by sslSocketFactory */
    string host = userUrl.noteStoreUrl.substr(
        sizeof("https://") - 1,
        userUrl.noteStoreUrl.find(".com") + 4 - (sizeof("https://") - 1));
    string path =
        userUrl.noteStoreUrl.substr(userUrl.noteStoreUrl.find(".com") + 4);
    shared_ptr<TSocket> noteStoreSSLSocket =
        sslSocketFactory->createSocket(host, 443);
    shared_ptr<TBufferedTransport> noteStoreBufferedTransport(
        new TBufferedTransport(noteStoreSSLSocket));
    shared_ptr<TTransport> noteStoreTransport(
        new THttpClient(noteStoreBufferedTransport, host, path));
    shared_ptr<TProtocol> noteStoreProtocol(
        new TBinaryProtocol(noteStoreTransport));
    NoteStoreClient noteStore(noteStoreProtocol);
    noteStoreTransport->open();

    /* sync from Evernote service */
    cout << "\nStart Synchronizing from server..." << endl;
    try {
        SyncState syncState;
        SyncChunkFilter filter;
        SyncChunk chunk;
        vector<SyncChunk> chunks;
        const int32_t maxEntries = 50;

        /*for incremental sync, init afterUSN by lastUpdateCount*/
        int32_t afterUSN = db->lastUpdateCount;
        cout << "(client)lastUpdateCount: " << afterUSN << endl;

        noteStore.getSyncState(syncState, g_AuthToken);
        cout << "(server)updateCount: " << syncState.updateCount << endl;

        /* as edam-sync.pdf suggested, buffer all the chunks to rebuild the
         * state of server, notebooks, notes, tags are what we care about,
         * resources is not counted because we will retrieve resources for each
         * note separately, note contents also have to be requested separately.
         */
        filter.__set_includeNotebooks(true);
        filter.__set_includeNotes(true);
        filter.__set_includeTags(true);
        filter.__set_includeExpunged(true);

        if (afterUSN < syncState.updateCount) {
            noteStore.getFilteredSyncChunk(chunk, g_AuthToken, afterUSN,
                                           maxEntries, filter);
            chunks.push_back(chunk);
            while (chunk.chunkHighUSN < chunk.updateCount) {
                afterUSN = chunk.chunkHighUSN;
                chunk.chunkHighUSN = 0;
                chunk.updateCount = 0;
                noteStore.getFilteredSyncChunk(chunk, g_AuthToken, afterUSN,
                                               maxEntries, filter);
                chunks.push_back(chunk);
            }
            afterUSN = chunk.chunkHighUSN;
        }

        vector<Notebook> notebooks;
        vector<Note> notes;
        vector<Tag> tags;
        vector<Guid> expungedNotebooks, expungedNotes, expungedTags;

        getServerUpdateInfo(chunks, notebooks, notes, tags, expungedNotebooks,
                            expungedNotes, expungedTags);

        /* fetch for the full content and resources for each note,
         * since getFilteredSyncChunk() only return the minimum information,
         * content and resources must be requested seperately. */
        for (auto i = notes.begin(); i != notes.end(); ++i) {
            /* getNote() is DEPRECATED!!! use getNoteWithResultSpec() */
            NoteResultSpec spec;
            spec.__set_includeContent(true);
            spec.__set_includeResourcesData(true);
            noteStore.getNoteWithResultSpec(*i, g_AuthToken, i->guid, spec);
        }

        /* update local database */
        updateTags(db, tags, expungedTags);
        updateNotebooks(db, notebooks, expungedNotebooks);
        updateNotes(db, notes, expungedNotes);

        /* save client variables */
        db->lastUpdateCount = afterUSN;
        db->lastSyncTime = time(NULL);
        db->close();
    } catch (TException &tx) {
        cout << "NoteStore ERROR: " << tx.what() << endl;
    }

    return 0;
}

int32_t updateNotebooks(shared_ptr<EvernoteDB> &db, vector<Notebook> &notebooks,
                        vector<string> &guids) {
    if (!db)
        return -1;

    for (auto i = notebooks.begin(); i != notebooks.end(); ++i) {
        db->updateNotebook(*i);
    }

    for (auto i = guids.begin(); i != guids.end(); ++i) {
        db->deleteNotebook(*i);
    }

    return 0;
}

int32_t updateNotes(shared_ptr<EvernoteDB> &db, vector<Note> &notes,
                    vector<string> &guids) {
    if (!db)
        return -1;

    for (auto i = notes.begin(); i != notes.end(); ++i) {
        db->updateNote(*i);
    }

    for (auto i = guids.begin(); i != guids.end(); ++i) {
        db->deleteNote(*i);
    }

    return 0;
}

int32_t updateTags(shared_ptr<EvernoteDB> &db, vector<Tag> &tags,
                   vector<string> &guids) {
    if (!db)
        return -1;

    for (auto i = tags.begin(); i != tags.end(); ++i) {
        db->updateTag(*i);
    }

    for (auto i = guids.begin(); i != guids.end(); ++i) {
        db->deleteTag(*i);
    }
}

static void getServerUpdateInfo(vector<SyncChunk> &chunks,
                                vector<Notebook> &notebooks,
                                vector<Note> &notes, vector<Tag> &tags,
                                vector<Guid> &expungedNotebooks,
                                vector<Guid> &expungedNotes,
                                vector<Guid> &expungedTags) {
    /* set<> is used to store Evernote data object, vector<> is not used
     * because when maxEntries is set too small, Evernote data ojbect
     * may be transmitted multiples times with different USN, we only
     * need the latest one, to strip the duplicated objects, we could
     * use the help of set<>. In order to comply with set<>, we must
     * implement a operator< method for the data type, it's wired that
     * thrift generated a stub operator< method for almost all the
     * Evernote data type, but with no implementation, so we have to
     * implement it by ourselves. */
    set<Notebook> _notebooks;
    set<Note> _notes;
    set<Tag> _tags;

    /*only guid is given for the expunged.*/
    set<Guid> _expungedNotebooks;
    set<Guid> _expungedNotes;
    set<string> _expungedTags; /* Guid is a typedef of string:) */

    for (auto i = chunks.begin(); i != chunks.end(); i++) {
        _notebooks.insert(i->notebooks.begin(), i->notebooks.end());
        _notes.insert(i->notes.begin(), i->notes.end());
        _tags.insert(i->tags.begin(), i->tags.end());

        _expungedNotebooks.insert(i->expungedNotebooks.begin(),
                                  i->expungedNotebooks.end());
        _expungedNotes.insert(i->expungedNotes.begin(), i->expungedNotes.end());
        _expungedTags.insert(i->expungedTags.begin(), i->expungedTags.end());
    }

    notebooks.insert(notebooks.end(), _notebooks.begin(), _notebooks.end());
    notes.insert(notes.end(), _notes.begin(), _notes.end());
    tags.insert(tags.end(), _tags.begin(), _tags.end());
    expungedNotebooks.insert(expungedNotebooks.end(),
                             _expungedNotebooks.begin(),
                             _expungedNotebooks.end());
    expungedNotes.insert(expungedNotes.end(), _expungedNotes.begin(),
                         _expungedNotes.end());
    expungedTags.insert(expungedTags.end(), _expungedTags.begin(),
                        _expungedTags.end());

    cout << "updated notebooks: " << endl;
    for (auto i = notebooks.begin(); i != notebooks.end(); ++i)
        cout << "\t" << i->name << endl;

    cout << "updated notes: " << endl;
    for (auto i = notes.begin(); i != notes.end(); ++i) {
        cout << "\t" << i->title << endl;
    }

    cout << "updated tags: " << endl;
    for (auto i = tags.begin(); i != tags.end(); ++i) {
        cout << "\t" << i->name << endl;
    }

    cout << "expunged notebook guids: " << endl;
    for (auto i = expungedNotebooks.begin(); i != expungedNotebooks.end(); ++i)
        cout << "\t" << *i << endl;

    cout << "expunged note guids: " << endl;
    for (auto i = expungedNotes.begin(); i != expungedNotes.end(); ++i)
        cout << "\t" << *i << endl;

    cout << "expunged tag guids: " << endl;
    for (auto i = expungedTags.begin(); i != expungedTags.end(); ++i)
        cout << "\t" << *i << endl;
}