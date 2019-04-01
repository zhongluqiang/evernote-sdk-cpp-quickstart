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
#include "tinyxml2.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::stdcxx;
using namespace evernote::edam;
using namespace std;
using namespace tinyxml2;

/* Sandbox: https://sandbox.evernote.com/
 * Evernote International 产品环境： https://www.evernote.com/
 * 印象笔记产品环境： https://app.yinxiang.com/
 * 印象笔记沙盒环境：https://sandbox.yinxiang.com
 */

#define HOST_YINXIANG "app.yinxiang.com"
#define HOST_EVERNOTE "www.evernote.com"
#define HOST_EVERNOTE_SANDBOX "sandbox.evernote.com"
#define HOST_YINXIANG_SANDBOX "sandbox.yinxiang.com"

const char *g_BaseUrl = "put your host url here";
const char *g_AuthToken = "put your dev token here";

const string g_DataDir("./evernote-data/");
XMLDocument *EvernoteXMLDataBaseInit(void);
int32_t getUpdateCount(XMLDocument *);
int32_t setUpdateCount(XMLDocument *, int32_t value);
int32_t updateNotebooks(XMLDocument *, vector<Notebook> &, vector<string> &);
int32_t updateNotes(NoteStoreClient &, XMLDocument *, vector<Note> &,
                    vector<string> &);
void closeXMLDataBase(XMLDocument *);
int32_t saveNote(const string &, Note &);

int main() {
    if (string(g_BaseUrl) == "put your host url here") {
        cerr << "Invalid host url, exit!!!" << endl;
        return -1;
    }
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
            cout << "<<" << i->name << ">> ";
        cout << endl;

        cout << "list tags              : ";
        for (auto i = tags.begin(); i != tags.end(); i++)
            cout << "\"" << i->name << "\" ";
        cout << endl;
    } catch (TException &tx) {
        cout << "NoteStore ERROR: " << tx.what() << endl;
        return -1;
    }

    /* open XML database before sync, we use a XML file to store Evernote
     * meta data of notes and notebooks, real note contents and resources
     * are not stored by XML, they will be stored as files at local. A
     * sample XML database will look like this:
     *   <?xml version="1.0" encoding="UTF-8" standalone="no"?>
     *   <db-evernote>
     *       <updateCount>186</updateCount>
     *       <notebooks>
     *           <notebook1>
     *               <note1>...</note1>
     *               <note2>...</note2>
     *               ...
     *           </notebook1>
     *           ...
     *       </notebooks>
     *   </db-evernote> */
    XMLDocument *db = EvernoteXMLDataBaseInit();
    if (!db) {
        cerr << "open XML database failed" << endl;
        throw TException("XML database error!");
    }

    /* sync notes and notebooks from Evernote service */
    cout << "\nStart Synchronizing from server..." << endl;
    SyncState syncState;
    SyncChunkFilter filter;
    vector<SyncChunk> chunks;
    try {
        SyncChunk tmpChunk;
        const int32_t maxEntries = 10;

        /*for incremental sync, read afterUSN from stored file*/
        int32_t afterUSN = getUpdateCount(db);
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

        cout << "expunged notebook guids: " << endl;
        for (auto i = expungedNotebooks.begin(); i != expungedNotebooks.end();
             i++)
            cout << "\t<<" << *i << ">>" << endl;

        cout << "expunged note guids: " << endl;
        for (auto i = expungedNotes.begin(); i != expungedNotes.end(); i++)
            cout << "\t<<" << *i << ">>" << endl;

        /* rebuild server status corresponding to the changes. */
        updateNotebooks(db, notebooks, expungedNotebooks);
        updateNotes(noteStore, db, notes, expungedNotes);

        /* for incremental sync, store afterUSN so next time we can
         * know from which point to start sync
         */
        setUpdateCount(db, afterUSN);
        closeXMLDataBase(db);

    } catch (TException &tx) {
        cout << "NoteStore ERROR: " << tx.what() << endl;
    }

    return 0;
}

/* On success, return a valid pointer of XML database, othrewise return NULL,
 * valid XML database must include a declaration and a db-evernote root
 * element */
XMLDocument *EvernoteXMLDataBaseInit(void) {
    XMLDocument *db = new XMLDocument();
    if (db) {
        if (db->LoadFile("./evernote-data/db.xml") == XML_SUCCESS) {
            XMLElement *root = db->RootElement();
            if (root && string("db-evernote") == root->Name()) {
                cout << "open XML database success." << endl;
                return db;
            }
        }

        /* if load file failed, assume there is no db exists, create a new
         * one and add a db-evernote root element*/
        cout << "open XML database failed, create a new one." << endl;
        const char *declaration =
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
        db->Parse(declaration);
        XMLElement *root = db->NewElement("db-evernote");
        db->InsertEndChild(root);
        return db;
    }

    return NULL;
}

/* On success, return the updateCount value, otherwise return 0*/
int32_t getUpdateCount(XMLDocument *db) {
    if (db) {
        XMLElement *root = db->RootElement();
        XMLElement *node = root->FirstChildElement("updateCount");
        if (node) {
            int32_t value;
            if (node->QueryIntText(&value) == XML_SUCCESS) {
                return value;
            }
        }
    }

    return 0;
}

/* On success, create an updateCount node under root node and set value text,
 * otherwise return -1*/
int32_t setUpdateCount(XMLDocument *db, int32_t value) {
    if (db) {
        XMLElement *root = db->RootElement();
        XMLElement *node = root->FirstChildElement("updateCount");
        if (node) {
            node->SetText(to_string(value).c_str());
            return 0;
        } else {
            node = db->NewElement("updateCount");
            node->SetText(to_string(value).c_str());
            root->InsertFirstChild(node);
        }
    }

    return -1;
}

int32_t updateNotebooks(XMLDocument *db, vector<Notebook> &notebooks,
                        vector<string> &guids) {
    if (!db)
        return -1;

    XMLElement *e;
    XMLElement *root = db->RootElement();
    XMLElement *eleNotebooks = root->FirstChildElement("notebooks");
    if (!eleNotebooks) {
        eleNotebooks = db->NewElement("notebooks");
        root->InsertEndChild(eleNotebooks);
    }

    /* check all the server returned updated notebooks, these notebooks
     * should be updated to local database. */
    for (auto i = notebooks.begin(); i != notebooks.end(); i++) {
        e = eleNotebooks->FirstChildElement();
        for (; e && e->Attribute("guid") != i->guid;
             e = e->NextSiblingElement())
            ;

        if (e && e->Attribute("guid") == i->guid) { /* found */
            cout << "update notebook: " << i->guid << ": ";
            cout << "<<" << e->Attribute("name") << ">> "
                 << "==> ";
            cout << "<<" << i->name << ">>" << endl;
            ;

            /* rename notebook directory */
            rename((g_DataDir + e->Attribute("name")).c_str(),
                   (g_DataDir + i->name).c_str());

            e->SetAttribute("name", i->name.c_str());
            e->SetAttribute("guid", i->guid.c_str());
        } else { /* not found, create a new one */
            cout << "create new notebook: "
                 << "<<" << i->name << ">>: ";
            cout << i->guid << endl;
            e = db->NewElement("notebook");
            e->SetAttribute("name", i->name.c_str());
            e->SetAttribute("guid", i->guid.c_str());
            eleNotebooks->InsertEndChild(e);
            /* create a directory for notebook */
            mkdir((g_DataDir + i->name).c_str(),
                  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
    }

    /* delete all the server returned expunged notebooks. */
    for (auto i = guids.begin(); i != guids.end(); i++) {
        e = eleNotebooks->FirstChildElement();
        for (; e && e->Attribute("guid") != (*i); e = e->NextSiblingElement())
            ;

        if (e && e->Attribute("guid") == *i) {
            cout << "delete notebook: "
                 << "<<" << e->Attribute("name") << ">>:" << *i << endl;

            /* delete notebook directory, must do before DeleteChild(),
             * or segmentation fault could occour due to the memory
             * free action of DeleteChild().
             * use "rm -fr" is dangerous, we will fix it later by creating
             * a removeDir() function. */
            system(("rm -fr " + g_DataDir + e->Attribute("name")).c_str());

            /* now we can delete the child element and free memeory */
            eleNotebooks->DeleteChild(e);
        }
    }

    return 0;
}

int32_t updateNotes(NoteStoreClient &noteStore, XMLDocument *db,
                    vector<Note> &notes, vector<string> &guids) {
    if (!db)
        return -1;

    /* Since in this program the notebooks were updated first, so we
     * will assume that we can always find the specific notebook that
     * a note belongs to in XML database. */
    XMLElement *e1, *e2;
    XMLElement *root = db->RootElement();
    XMLElement *eleNotebooks = root->FirstChildElement("notebooks");
    Note note;

    /* check all the updated notes, these notes should be updated to
     * local XML database. Note that trashed notes will not be removed,
     * compare to the normal note, the only difference for trashed note
     * is the active filed is set to false. */
    for (auto i = notes.begin(); i != notes.end(); i++) {
        // find the notebooks for the note
        for (e1 = eleNotebooks->FirstChildElement();
             e1 && e1->Attribute("guid") != i->notebookGuid;
             e1 = e1->NextSiblingElement())
            ;

        if (!e1) { /* not likely to happen, just print a warning message */
            cerr << " WARNING: found an orphan note: " << i->title << endl;
        } else {
            // find the note, if not, create a new one
            for (e2 = e1->FirstChildElement();
                 e2 && e2->Attribute("guid") != i->guid;
                 e2 = e2->NextSiblingElement())
                ;

            if (e2) { /* update the note */
                cout << "update note: " << i->title << endl;

                /* delete old note on the disk*/
                remove((g_DataDir + e1->Attribute("name") + "/" +
                        e2->Attribute("title") + ".enml")
                           .c_str());
                /* save note, with .enml suffix */
                noteStore.getNote(note, g_AuthToken, i->guid, true, false,
                                  false, false);
                saveNote(g_DataDir + e1->Attribute("name"), note);

                e2->SetAttribute("title", i->title.c_str());
                e2->SetAttribute("guid", i->guid.c_str());
                e2->SetAttribute("active", i->active ? "true" : "false");
            } else { /* create a new note in this notebook */
                cout << "create new note: " << i->title << endl;
                e2 = db->NewElement("note");
                e2->SetAttribute("title", i->title.c_str());
                e2->SetAttribute("guid", i->guid.c_str());
                e2->SetAttribute("active", i->active ? "true" : "false");

                /* save note, with .enml suffix */
                noteStore.getNote(note, g_AuthToken, i->guid, true, false,
                                  false, false);
                saveNote(g_DataDir + e1->Attribute("name"), note);

                e1->InsertEndChild(e2);
            }
        }
    }

    /* check all the expunged notes, these notes should be remove from
     * local XML database. */
    for (auto i = guids.begin(); i != guids.end(); i++) {
        for (e1 = eleNotebooks->FirstChildElement(); e1 != NULL;
             e1 = e1->NextSiblingElement()) {
            for (e2 = e1->FirstChildElement(); e2 != NULL;
                 e2 = e2->NextSiblingElement()) {
                if (e2->Attribute("guid") == (*i)) {
                    cout << "delete note: " << (*i) << endl;

                    /* perform remove() before free element memory. */
                    remove((g_DataDir + e1->Attribute("name") + "/" +
                            e2->Attribute("title") + ".enml")
                               .c_str());

                    e1->DeleteChild(e2);
                }
            }
        }
    }

    return 0;
}

void closeXMLDataBase(XMLDocument *db) {
    db->SaveFile("./evernote-data/db.xml");
}

int32_t saveNote(const string &location, Note &note) {
    string filename = location + "/" + note.title + ".enml";
    ofstream file(filename);
    if (file) {
        file << note.content;
        file.close();
        return 0;
    } else {
        cerr << "create file " << filename << " failed!" << endl;
        return -1;
    }
}