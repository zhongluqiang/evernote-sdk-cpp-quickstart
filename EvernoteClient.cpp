#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/stdcxx.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/THttpClient.h>
#include <thrift/transport/TSSLSocket.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>

#include "gen-cpp/NoteStore.h"
#include "gen-cpp/NoteStore_types.h"
#include "gen-cpp/UserStore.h"
#include "gen-cpp/UserStore_constants.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

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

const char *g_BaseUrl = HOST_YINXIANG_SANDBOX;
const char *g_AuthToken = "put your dev token here";

int readUpdateCount(void);
void writeUpdateCount(int);

int main() {
    if (string(g_AuthToken) == "put your dev token here") {
        cerr << "Invalid Token, exit!!!" << endl;
        return -1;
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

    /* dump some UserStore information */
    User user;
    UserUrls userUrl;
    try {
        userStore.getUser(user, g_AuthToken);
        userStore.getUserUrls(userUrl, g_AuthToken);

        cout << "username       : " << user.username << endl;
        cout << "userid         : " << user.id << endl;
        cout << "email          : " << user.email << endl;
        cout << "privilege level: " << user.privilege << endl;
        cout << "create time    : " << user.created << endl;
        cout << "user url       : " << userUrl.userStoreUrl << endl;
        cout << "note url       : " << userUrl.noteStoreUrl << endl;
    } catch (TException &tx) {
        cout << "UserStore ERROR: " << tx.what() << endl;
        return -1;
    }

    /* use SSLSocket factory to create a NoteStoreClient */
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

    /* dump some NoteStore information */
    Notebook defaultNotebook;
    vector<Notebook> notebooks;
    vector<Tag> tags;
    try {
        noteStore.getDefaultNotebook(defaultNotebook, g_AuthToken);
        noteStore.listNotebooks(notebooks, g_AuthToken);
        noteStore.listTags(tags, g_AuthToken);

        cout << "default notebook       : " << defaultNotebook.name << endl;
        cout << "default notebook guid  : " << defaultNotebook.guid << endl;

        cout << "list notebooks         : ";
        for (auto i = notebooks.begin(); i != notebooks.end(); i++)
            cout << " <<" << i->name << ">> ";
        cout << endl;

        cout << "list tags              : ";
        for (auto i = tags.begin(); i != tags.end(); i++)
            cout << " \"" << i->name << "\" ";
        cout << endl;
    } catch (TException &tx) {
        cout << "NoteStore ERROR: " << tx.what() << endl;
        return -1;
    }

    /* sync notes and notebooks from Evernote service */
    cout << "\nStart Synchronizing form server..." << endl;
    SyncState syncState;
    SyncChunkFilter filter;
    vector<SyncChunk> chunks;
    try {
        SyncChunk tmpChunk;
        const int32_t maxEntries = 10;

        /*for incremental sync, read afterUSN from stored file*/
        int32_t afterUSN = readUpdateCount();
        cout << "lastUpdateCount: " << afterUSN << endl;

        noteStore.getSyncState(syncState, g_AuthToken);
        cout << "updateCount: " << syncState.updateCount << endl;

        /* buffer all the chunks to rebuild the state of server,
         * at this time we only care about the notebooks and notes,
         * also you should notice that note contents and resources will not be
         * retrieved, they must be requested separately.
         */
        filter.__set_includeNotebooks(true);
        filter.__set_includeNotes(true);
        filter.__set_includeNoteResources(true);
        /*if you want to filter out what has been permanently deleted,
        set this to true*/
        filter.__set_includeExpunged(true);
        noteStore.getFilteredSyncChunk(tmpChunk, g_AuthToken, afterUSN,
                                       maxEntries, filter);
        while (afterUSN < syncState.updateCount) {
            chunks.push_back(tmpChunk);
            afterUSN = tmpChunk.chunkHighUSN;
            tmpChunk.chunkHighUSN = 0;
            noteStore.getFilteredSyncChunk(tmpChunk, g_AuthToken, afterUSN,
                                           maxEntries, filter);
        }

        /* search through all the chunks to get the collection of updated
         * and expunged notes and notebooks, we need these information to
         * rebuild server status.*/
        vector<Notebook> notebooks;
        vector<Note> notes;
        vector<string>
            expungedNotebooks; /*only guid is given for the expunged.*/
        vector<string> expungedNotes;
        for (auto i = chunks.begin(); i != chunks.end(); i++) {
            for (auto j = i->notebooks.begin(); j != i->notebooks.end(); j++)
                notebooks.push_back(*j);
            for (auto j = i->notes.begin(); j != i->notes.end(); j++)
                notes.push_back(*j);
            for (auto j = i->expungedNotebooks.begin();
                 j != i->expungedNotebooks.end(); j++)
                expungedNotebooks.push_back(*j);
            for (auto j = i->expungedNotes.begin(); j != i->expungedNotes.end();
                 j++)
                expungedNotes.push_back(*j);
        }

        /* dump all the related data objects information. Expunged objects
         * are represented by guids. */
        cout << "changed notebooks: " << endl;
        for (auto i = notebooks.begin(); i != notebooks.end(); i++)
            cout << "\t<<" << i->name << ">>"
                 << ":" << i->guid << " " << endl;

        cout << "changed notes: " << endl;
        for (auto i = notes.begin(); i != notes.end(); i++)
            cout << "\t" << i->title << endl;

        cout << "expunged notebooks guids: " << endl;
        for (auto i = expungedNotebooks.begin(); i != expungedNotebooks.end();
             i++)
            cout << "\t<<" << *i << ">>" << endl;

        cout << "expunged notes: " << endl;
        for (auto i = expungedNotes.begin(); i != expungedNotes.end(); i++)
            cout << "\t<<" << *i << ">>" << endl;

        /* rebuild server status corresponding to the changes. */
        // todo...

        /* for incremental sync, store afterUSN so next time we can
         * know from which point to start sync
         */
        writeUpdateCount(afterUSN);
    } catch (TException &tx) {
        cout << "NoteStore ERROR: " << tx.what() << endl;
    }

    return 0;
}

int32_t readUpdateCount(void) {
    int value = 0;
    char tmp[20] = {0};

    FILE *file = fopen("./evernote-data/updateCount.txt", "r");
    if (file) {
        fread(tmp, 1, sizeof(tmp), file);
        sscanf(tmp, "%d", &value);
        fclose(file);
    }
    return value;
}

void writeUpdateCount(int32_t value) {
    char tmp[20] = {0};

    sprintf(tmp, "%d", value);
    FILE *file = fopen("./evernote-data/updateCount.txt", "w+");
    if (file) {
        fwrite(tmp, 1, strlen(tmp), file);
        fclose(file);
    }
}