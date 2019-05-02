#include "xmlUtil.h"
#include "enml2html.h"
#include "filesystem.h"
#include <fstream>
#include <iostream>

string EvernoteExporter::byte2hex(const char *data, int len) {
    const char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    string str(len * 2, ' ');
    for (int i = 0; i < len; i++) {
        str[i * 2] = hexmap[(data[i] & 0xF0) >> 4];
        str[i * 2 + 1] = hexmap[data[i] & 0x0f];
    }
    return str;
}

void EvernoteExporter::InsertEndChildByName(XMLDocument *doc, XMLElement *root,
                                            const string &name, int32_t value) {
    if (!doc || !root) {
        cerr << "invalid argument" << endl;
        return;
    }
    XMLElement *node = doc->NewElement(name.c_str());
    if (!node) {
        cerr << "NewElement() failed" << endl;
        return;
    }

    node->SetText(value);
    root->InsertEndChild(node);
}

void EvernoteExporter::InsertEndChildByName(XMLDocument *doc, XMLElement *root,
                                            const string &name, int64_t value) {
    if (!doc || !root) {
        cerr << "invalid argument" << endl;
        return;
    }
    XMLElement *node = doc->NewElement(name.c_str());
    if (!node) {
        cerr << "NewElement() failed" << endl;
        return;
    }

    node->SetText(value);
    root->InsertEndChild(node);
}

void EvernoteExporter::InsertEndChildByName(XMLDocument *doc, XMLElement *root,
                                            const string &name, bool value) {
    if (!doc || !root) {
        cerr << "invalid argument" << endl;
        return;
    }
    XMLElement *node = doc->NewElement(name.c_str());
    if (!node) {
        cerr << "NewElement() failed" << endl;
        return;
    }

    node->SetText(value);
    root->InsertEndChild(node);
}

void EvernoteExporter::InsertEndChildByName(XMLDocument *doc, XMLElement *root,
                                            const string &name, string &value) {
    if (!doc || !root) {
        cerr << "invalid argument" << endl;
        return;
    }
    XMLElement *node = doc->NewElement(name.c_str());
    if (!node) {
        cerr << "NewElement() failed" << endl;
        return;
    }

    node->SetText(value.c_str());
    root->InsertEndChild(node);
}

void EvernoteExporter::InsertEndChildByName(XMLDocument *doc, XMLElement *root,
                                            const string &name,
                                            Notebook &notebook) {
    if (!doc || !root) {
        cerr << "invalid argument" << endl;
        return;
    }
    XMLElement *node = doc->NewElement(name.c_str());
    if (!node) {
        cerr << "NewElement() failed" << endl;
        return;
    }

    InsertEndChildByName(doc, node, "name", notebook.name);
    InsertEndChildByName(doc, node, "guid", notebook.guid);
    InsertEndChildByName(doc, node, "updateSequenceNum",
                         notebook.updateSequenceNum);
    InsertEndChildByName(doc, node, "defaultNotebook",
                         notebook.defaultNotebook);
    InsertEndChildByName(doc, node, "serviceCreated", notebook.serviceCreated);
    InsertEndChildByName(doc, node, "serviceUpdated", notebook.serviceUpdated);

    root->InsertEndChild(node);
}
void EvernoteExporter::InsertEndChildByName(XMLDocument *db, XMLElement *root,
                                            const string &name,
                                            Resource &resource) {
    if (!db || !root) {
        cerr << "invalid argument" << endl;
        return;
    }
    XMLElement *node = db->NewElement(name.c_str());
    if (!node) {
        cerr << "NewElement() failed" << endl;
        return;
    }

    InsertEndChildByName(db, node, "guid", resource.guid);
    InsertEndChildByName(db, node, "noteGuid", resource.noteGuid);
    InsertEndChildByName(db, node, "mime", resource.mime);
    InsertEndChildByName(db, node, "updateSequenceNum",
                         resource.updateSequenceNum);
    InsertEndChildByName(db, node, "size", resource.data.size);
    string bodyHash =
        byte2hex(resource.data.bodyHash.c_str(), resource.data.bodyHash.size());
    InsertEndChildByName(db, node, "bodyHash", bodyHash);

    if (resource.width != 0 && resource.height != 0) {
        InsertEndChildByName(db, node, "width", resource.width);
        InsertEndChildByName(db, node, "height", resource.height);
    }

    if (resource.attributes.fileName.size() > 0) {
        InsertEndChildByName(db, node, "fileName",
                             resource.attributes.fileName);
    }

    root->InsertEndChild(node);
}
void EvernoteExporter::InsertEndChildByName(XMLDocument *db, XMLElement *root,
                                            const string &name, Note &note) {
    if (!db || !root) {
        cerr << "invalid argument" << endl;
        return;
    }
    XMLElement *node = db->NewElement(name.c_str());
    if (!node) {
        cerr << "NewElement() failed" << endl;
        return;
    }

    InsertEndChildByName(db, node, "title", note.title);
    InsertEndChildByName(db, node, "guid", note.guid);
    InsertEndChildByName(db, node, "notebookGuid", note.notebookGuid);
    InsertEndChildByName(db, node, "active", note.active);
    InsertEndChildByName(db, node, "updateSequenceNum", note.updateSequenceNum);
    InsertEndChildByName(db, node, "contentLength", note.contentLength);
    InsertEndChildByName(db, node, "created", note.created);
    InsertEndChildByName(db, node, "updated", note.updated);
    InsertEndChildByName(db, node, "deleted", note.deleted);

    for (auto i = note.tagGuids.begin(); i != note.tagGuids.end(); ++i) {
        InsertEndChildByName(db, node, "tag", this->mapGuid2Tag[*i].name);
    }

    for (auto i = note.resources.begin(); i != note.resources.end(); ++i) {
        InsertEndChildByName(db, node, "resource", *i);
    }

    root->InsertEndChild(node);
}

void EvernoteExporter::InsertEndChildByName(XMLDocument *doc, XMLElement *root,
                                            const string &name,
                                            NotebookTree &notebookTree) {
    if (!doc || !root) {
        cerr << "invalid argument" << endl;
        return;
    }
    XMLElement *node = doc->NewElement(name.c_str());
    if (!node) {
        cerr << "NewElement() failed" << endl;
        return;
    }

    InsertEndChildByName(doc, node, "name", notebookTree.notebook.name);
    InsertEndChildByName(doc, node, "guid", notebookTree.notebook.guid);
    InsertEndChildByName(doc, node, "updateSequenceNum",
                         notebookTree.notebook.updateSequenceNum);
    InsertEndChildByName(doc, node, "defaultNotebook",
                         notebookTree.notebook.defaultNotebook);
    InsertEndChildByName(doc, node, "serviceCreated",
                         notebookTree.notebook.serviceCreated);
    InsertEndChildByName(doc, node, "serviceUpdated",
                         notebookTree.notebook.serviceUpdated);

    for (auto i = notebookTree.notes.begin(); i != notebookTree.notes.end();
         ++i) {
        InsertEndChildByName(doc, node, "note", *i);
    }

    root->InsertEndChild(node);
}

void EvernoteExporter::init(list<Notebook> &notebooks, list<Note> &notes,
                            list<Tag> &tags) {
    for (auto i = notebooks.begin(); i != notebooks.end(); ++i) {
        NotebookTree tree;
        tree.notebook = *i;
        for (auto j = notes.begin(); j != notes.end(); ++j) {
            if (j->notebookGuid == i->guid) {
                tree.notes.push_back(*j);
            }
        }
        this->tree.notebooks.push_back(tree);
    }

    for (auto i = tags.begin(); i != tags.end(); ++i) {
        mapGuid2Tag[i->guid] = *i;
    }

    totalNotebooks = notebooks.size();
    totalNotes = notes.size();
    totalTags = tags.size();
}

void EvernoteExporter::exportAll() {
    exportENML();
    exportHTML();
    exportXMLIndex();
    exportHTMLIndex();
}

void EvernoteExporter::exportXMLIndex() {
    XMLDocument *doc = new XMLDocument();
    if (!doc) {
        cerr << "new XMLdocument() failed!!!" << endl;
        return;
    }
    tinyxml2_closer closer(doc);
    /* add declaration and create a evernote root node */
    const char *declaration =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    doc->Parse(declaration);
    XMLElement *root = doc->NewElement("evernote");
    doc->InsertEndChild(root);

    InsertEndChildByName(doc, root, "totalNotebooks", this->totalNotebooks);
    InsertEndChildByName(doc, root, "totalNotes", this->totalNotes);
    InsertEndChildByName(doc, root, "totalTags", this->totalTags);

    for (auto i = this->tree.notebooks.begin(); i != this->tree.notebooks.end();
         ++i) {
        InsertEndChildByName(doc, root, "notebook", *i);
    }

    doc->SaveFile(this->xml_index.c_str());
}
void EvernoteExporter::exportHTMLIndex() {
    XMLDocument *doc = new XMLDocument();
    if (!doc) {
        cerr << "new XMLdocument() failed!!!" << endl;
        return;
    }
    tinyxml2_closer closer(doc);
    /* add <!DOCTYPE html> declaration  */
    const char *declaration = "<!DOCTYPE html>";
    doc->Parse(declaration);
    /* add <html> root tag */
    XMLElement *root = doc->NewElement("html");
    doc->InsertEndChild(root);
    /*add <head> tag*/
    XMLElement *head = doc->NewElement("head");
    root->InsertEndChild(head);
    /* add <body> tag*/
    XMLElement *body = doc->NewElement("body");
    root->InsertEndChild(body);

    XMLElement *meta = doc->NewElement("meta");
    meta->SetAttribute("charset", "utf-8");
    head->InsertEndChild(meta);
    string title = "note list";
    InsertEndChildByName(doc, head, "title", title);

    for (auto i = this->tree.notebooks.begin(); i != this->tree.notebooks.end();
         ++i) {
        InsertEndChildByName(doc, body, "h1", i->notebook.name);
        for (auto j = i->notes.begin(); j != i->notes.end(); ++j) {
            XMLElement *a = doc->NewElement("a");
            string href =
                i->notebook.name + "/" + j->title + "/" + j->title + ".html";
            a->SetAttribute("href", href.c_str());
            a->SetText(j->title.c_str());
            body->InsertEndChild(a);
            XMLElement *br = doc->NewElement("br");
            body->InsertEndChild(br);
        }
    }

    doc->SaveFile(this->html_index.c_str());
}

void EvernoteExporter::saveNoteResources(const string &dir,
                                         vector<Resource> &resources) {
    for (auto i = resources.begin(); i != resources.end(); ++i) {
        if (i->data.body.size() > 0) {
            string filename =
                dir + "/" +
                byte2hex(i->data.bodyHash.c_str(), i->data.bodyHash.size());
            filename += "." + i->mime.substr(i->mime.find("/") + 1);
            ofstream file(filename, std::ofstream::binary);
            if (file) {
                file.write(i->data.body.data(), i->data.body.size());
                file.close();
            }
        }
    }
}

void EvernoteExporter::exportNoteENML(const string &notebookdir, Note &note) {
    string notedir = notebookdir + "/" + note.title;
    create_directory(notedir);
    string filename = notedir + "/" + note.title + ".enml";
    ofstream file(filename);
    if (file) {
        file << note.content;
        file.close();
    }

    saveNoteResources(notedir, note.resources);
}

void EvernoteExporter::exportNoteHTML(const string &notebookdir, Note &note) {
    string notedir = notebookdir + "/" + note.title;
    create_directory(notedir);
    string filename = notedir + "/" + note.title + ".html";
    string content = note.content;
    string html;
    enml2html(note.title, content, html);
    ofstream file(filename);
    if (file) {
        file << html;
        file.close();
    }
    saveNoteResources(notedir, note.resources);
}

void EvernoteExporter::exportENML() {
    for (auto i = this->tree.notebooks.begin(); i != this->tree.notebooks.end();
         ++i) {
        string notebookdir = this->enml_basedir + "/" + i->notebook.name;
        if (exists(notebookdir)) {
            remove_all(notebookdir);
        }
        create_directories(notebookdir);

        for (auto j = i->notes.begin(); j != i->notes.end(); ++j) {
            exportNoteENML(notebookdir, *j);
        }
    }
}

void EvernoteExporter::exportHTML() {
    for (auto i = this->tree.notebooks.begin(); i != this->tree.notebooks.end();
         ++i) {
        string notebookdir = this->html_basedir + "/" + i->notebook.name;
        if (exists(notebookdir)) {
            remove_all(notebookdir);
        }
        create_directories(notebookdir);
        for (auto j = i->notes.begin(); j != i->notes.end(); ++j) {
            exportNoteHTML(notebookdir, *j);
        }
    }
}