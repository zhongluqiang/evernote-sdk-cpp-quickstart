#include <boost/filesystem.hpp>
#include <string>
#include <vector>
/* hope this will change to fs = std::filesystem someday */
namespace fs = boost::filesystem;

void copy(const std::string &from, const std::string &to) {
    fs::copy(from, to);
}

void copy_file(const std::string &from, const std::string &to) {
    fs::copy_file(from, to);
}

void rename(const std::string &frompath, const std::string &topath) {
    fs::rename(frompath, topath);
}

void remove(const std::string &p) { fs::remove(p); }

void remove_all(const std::string &p) { fs::remove_all(p); }

bool create_directory(const std::string &p) { return fs::create_directory(p); }

bool create_directories(const std::string &p) {
    return fs::create_directories(p);
}

bool exists(const std::string &p) { return fs::exists(p); }

bool is_directory(const std::string &p) { return fs::is_directory(p); }

bool is_regular(const std::string &p) { return fs::is_regular(p); }

long long file_size(const std::string &p) { return fs::file_size(p); }

bool is_empty(const std::string &p) { return fs::is_empty(p); }

bool list_files(const std::string &p, std::vector<std::string> &files) {
    fs::path dir(p);
    if (fs::exists(dir) && fs::is_directory(dir)) {
        for (fs::directory_iterator i(dir); i != fs::directory_iterator();
             ++i) {
            if (fs::is_regular(i->status())) {
                files.push_back(i->path().string());
            }
        }
        return true;
    }

    return false;
}