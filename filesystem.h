#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

/*
 * Filesystem wrapper
 */
#include <string>
#include <vector>
void copy(const std::string &from, const std::string &to);
void copy_file(const std::string &from, const std::string &to);
void rename(const std::string &old_p, const std::string &new_p);
void remove(const std::string &p);
void remove_all(const std::string &p);
bool create_directory(const std::string &p);
bool create_directories(const std::string &p);
bool exists(const std::string &p);
bool is_directory(const std::string &p);
bool is_regular(const std::string &p);
long long file_size(const std::string &p);
bool is_empty(const std::string &p);
bool list_files(const std::string &p, std::vector<std::string> &files);

#endif