#ifndef __XMLUTIL_H__
#define __XMLUTIL_H__

#include "gen-cpp/NoteStore_types.h"
#include "gen-cpp/Types_types.h"
#include "gen-cpp/UserStore_types.h"
#include "tinyxml2.h"
using namespace evernote::edam;
using namespace std;
using namespace tinyxml2;

class tinyxml2_closer {
    XMLDocument *_doc;

  public:
    tinyxml2_closer(XMLDocument *doc) : _doc(doc) {}
    ~tinyxml2_closer() {
        if (_doc) {
            _doc->Clear();
            delete _doc;
        }
    }
};

class NotebookTree {
  public:
    Notebook notebook;
    vector<Note> notes;
};

class EvernoteTree {
  public:
    vector<NotebookTree> notebooks;
};

class EvernoteExporter {
  public:
    EvernoteExporter(string _basedir) : basedir(_basedir) {}
    void init(list<Notebook> &, list<Note> &, list<Tag> &);
    void exportAll();

  private:
    int32_t totalNotebooks, totalNotes, totalTags;
    EvernoteTree tree;
    map<Guid, Tag> mapGuid2Tag;
    string basedir;
    string enml_basedir = basedir + "/enml/";
    string html_basedir = basedir + "/html/";
    string xml_index = enml_basedir + "/index.xml";
    string html_index = html_basedir + "/index.html";

    void exportXMLIndex();
    void exportHTMLIndex();
    void exportENML();
    void exportHTML();
    void exportNoteENML(const string &, Note &);
    void exportNoteHTML(const string &, Note &);
    void saveNoteResources(const string &, vector<Resource> &);

    string byte2hex(const char *, int);
    void InsertEndChildByName(XMLDocument *, XMLElement *, const string &,
                              int32_t);
    void InsertEndChildByName(XMLDocument *, XMLElement *, const string &,
                              int64_t);
    void InsertEndChildByName(XMLDocument *, XMLElement *, const string &,
                              bool);
    void InsertEndChildByName(XMLDocument *, XMLElement *, const string &,
                              string &);
    void InsertEndChildByName(XMLDocument *, XMLElement *, const string &,
                              Notebook &);
    void InsertEndChildByName(XMLDocument *, XMLElement *, const string &,
                              Note &);
    void InsertEndChildByName(XMLDocument *, XMLElement *, const string &,
                              Resource &);
    void InsertEndChildByName(XMLDocument *, XMLElement *, const string &,
                              NotebookTree &);
};
#endif