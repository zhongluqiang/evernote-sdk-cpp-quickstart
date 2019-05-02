#ifndef DATABASE_H
#define DATABASE_H
#include "gen-cpp/Types_types.h"

using namespace evernote::edam;
using namespace std;

class EvernoteDB {
  public:
    EvernoteDB(string _basedir)
        : lastUpdateCount(0), lastSyncTime(0), basedir(_basedir) {}
    ~EvernoteDB() throw() {}

    int32_t lastUpdateCount;
    Timestamp lastSyncTime;

    list<Notebook> notebooks;
    list<Note> notes;
    list<Tag> tags;

    int32_t open();
    int32_t close();

    int32_t updateNotebook(Notebook &);
    int32_t deleteNotebook(Guid &);

    int32_t updateNote(Note &);
    int32_t deleteNote(Guid &);

    int32_t updateTag(Tag &);
    int32_t deleteTag(Guid &);

  private:
    string basedir;
    string client_variable_file = basedir + "/variable.txt";
    string notebooks_dir = basedir + "/notebooks/";
    string notes_dir = basedir + "/notes/";
    string tags_dir = basedir + "/tags/";
    string export_dir = basedir + "/export/";

    void loadLocalNotebooks();
    void loadLocalNotes();
    void loadLocalTags();
    void updateLocalNotebook(Notebook &);
    void deleteLocalNotebook(Guid &);
    void updateLocalNote(Note &);
    void deleteLocalNote(Guid &);
    void updateLocalTag(Tag &);
    void deleteLocalTag(Guid &);

    /* export note to raw ENML file and create an overall XML index file */
    void exportAll();
};
#endif