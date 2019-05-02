#include "Database.h"
#include "filesystem.h"
#include "xmlUtil.h"
#include <fstream>
#include <iostream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSimpleFileTransport.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int32_t EvernoteDB::open() {

    if (!exists(client_variable_file))
        return -1;

    fstream file(client_variable_file, ios::in);
    if (!file)
        return -1;

    string s1, s2;
    file >> s1 >> this->lastUpdateCount >> s2 >> this->lastSyncTime;
    file.close();

    loadLocalNotebooks();
    loadLocalNotes();
    loadLocalTags();

    return 0;
}

int32_t EvernoteDB::close() {
    fstream file(client_variable_file, ios::out | ios::trunc);
    if (!file)
        return -1;

    file << "lastUpdateCount: " << this->lastUpdateCount << " "
         << "lastSyncTime: " << this->lastSyncTime;
    file.close();

    exportAll();

    return -1;
}

int32_t EvernoteDB::updateNotebook(Notebook &notebook) {
    auto i = this->notebooks.begin();
    for (; i != this->notebooks.end(); ++i) {
        if (i->guid == notebook.guid) {
            *i = notebook;
        }
    }

    if (i == this->notebooks.end()) {
        this->notebooks.push_back(notebook);
    }

    updateLocalNotebook(notebook);

    return 0;
}

int32_t EvernoteDB::deleteNotebook(Guid &guid) {
    auto i = this->notebooks.begin();
    for (; i != this->notebooks.end(); ++i) {
        if (i->guid == guid) {
            this->notebooks.erase(i);
            break;
        }
    }
    if (i == this->notebooks.end()) {
        cout << "ignore expunged notebook guid:" << guid << endl;
    }

    deleteLocalNotebook(guid);

    return 0;
}

int32_t EvernoteDB::updateNote(Note &note) {
    auto i = this->notes.begin();
    for (; i != this->notes.end(); ++i) {
        if (i->guid == note.guid) {
            *i = note;
        }
    }

    if (i == this->notes.end()) {
        this->notes.push_back(note);
    }

    updateLocalNote(note);

    return 0;
}

int32_t EvernoteDB::deleteNote(Guid &guid) {
    auto i = this->notes.begin();
    for (; i != this->notes.end(); ++i) {
        if (i->guid == guid) {
            this->notes.erase(i);
            break;
        }
    }

    if (i == this->notes.end()) {
        cout << "ignore expunged note guid:" << guid << endl;
    }

    deleteLocalNote(guid);

    return 0;
}

int32_t EvernoteDB::updateTag(Tag &tag) {
    auto i = this->tags.begin();
    for (; i != this->tags.end(); ++i) {
        if (i->guid == tag.guid) {
            *i = tag;
        }
    }

    if (i == this->tags.end()) {
        this->tags.push_back(tag);
    }

    updateLocalTag(tag);

    return 0;
}

int32_t EvernoteDB::deleteTag(Guid &guid) {
    auto i = this->tags.begin();
    for (; i != this->tags.end(); ++i) {
        if (i->guid == guid) {
            this->tags.erase(i);
            break;
        }
    }

    if (i == this->tags.end()) {
        cout << "ignore expunged tag guid:" << guid << endl;
    }

    deleteLocalTag(guid);

    return 0;
}

void EvernoteDB::loadLocalNotebooks() {
    shared_ptr<TTransport> trans;
    vector<string> notebooks;

    list_files(notebooks_dir, notebooks);
    for (auto i = notebooks.begin(); i != notebooks.end(); ++i) {
        trans.reset(new TSimpleFileTransport(*i, true, true));
        TBinaryProtocol proto(trans);
        Notebook notebook;
        notebook.read(&proto);
        this->notebooks.push_back(notebook);
    }
}

void EvernoteDB::loadLocalNotes() {
    shared_ptr<TTransport> trans;
    vector<string> notes;

    list_files(notes_dir, notes);
    for (auto i = notes.begin(); i != notes.end(); ++i) {
        trans.reset(new TSimpleFileTransport(*i, true, true));
        TBinaryProtocol proto(trans);
        Note note;
        note.read(&proto);
        this->notes.push_back(note);
    }
}

void EvernoteDB::loadLocalTags() {
    shared_ptr<TTransport> trans;
    vector<string> tagGuids;

    list_files(this->tags_dir, tagGuids);
    for (auto i = tagGuids.begin(); i != tagGuids.end(); ++i) {
        trans.reset(new TSimpleFileTransport(*i, true, true));
        TBinaryProtocol proto(trans);
        Tag tag;
        tag.read(&proto);
        this->tags.push_back(tag);
    }
}

void EvernoteDB::updateLocalNotebook(Notebook &notebook) {
    string filename = this->notebooks_dir + notebook.guid;
    if (exists(filename)) {
        remove(filename);
    }
    shared_ptr<TTransport> trans(
        new TSimpleFileTransport(filename, true, true));
    TBinaryProtocol proto(trans);
    notebook.write(&proto);
}

void EvernoteDB::deleteLocalNotebook(Guid &guid) {
    string filename = this->notebooks_dir + guid;
    if (exists(filename)) {
        remove(filename);
    }
}

void EvernoteDB::updateLocalNote(Note &note) {
    string filename = this->notes_dir + note.guid;
    if (exists(filename)) {
        remove(filename);
    }
    shared_ptr<TTransport> trans(
        new TSimpleFileTransport(filename, true, true));
    TBinaryProtocol proto(trans);
    note.write(&proto);
}

void EvernoteDB::deleteLocalNote(Guid &guid) {
    string filename = this->notes_dir + guid;
    if (exists(filename)) {
        remove(filename);
    }
}

void EvernoteDB::updateLocalTag(Tag &tag) {
    string filename = this->tags_dir + tag.guid;
    if (exists(filename)) {
        remove(filename);
    }
    shared_ptr<TTransport> trans(
        new TSimpleFileTransport(filename, true, true));
    TBinaryProtocol proto(trans);
    tag.write(&proto);
}

void EvernoteDB::deleteLocalTag(Guid &guid) {
    string filename = this->tags_dir + guid;
    if (exists(filename)) {
        remove(filename);
    }
}

void EvernoteDB::exportAll() {
    if (exists(this->export_dir)) {
        remove_all(this->export_dir);
    }
    create_directory(this->export_dir);

    shared_ptr<EvernoteExporter> exporter(
        new EvernoteExporter(this->export_dir));
    exporter->init(this->notebooks, this->notes, this->tags);
    exporter->exportAll();
}