#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

void get_md5_hash(const std::string str, std::string &result);

void parse_html_uri(const std::string& s, std::vector<std::string>& parsed);

std::string get_longest_extension_from_filename(const std::string s);

std::string get_name_wo_extension_from_filename(const std::string s);

#endif // UTIL_H
