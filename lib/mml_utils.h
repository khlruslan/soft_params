#pragma once
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace mml {

struct SearchResult {
  std::vector<std::string> data_;
};

struct TrimResult {
  std::vector<std::string> data_;
};

struct MapStringString {
  std::map<std::string, std::string> data_;
};

struct VectorMapStringString {
  std::vector<MapStringString> data_;
};

struct ConvertInfo {
  mml::MapStringString type_to_number_;
  mml::MapStringString type_to_value_;
  std::string type_key_;
};

std::string_view rtrim(std::string_view line, const std::set<char> &chars);
std::string_view ltrim(std::string_view line, const std::set<char> &chars);
std::string_view trim(std::string_view line, const std::set<char> &chars);

std::vector<std::string_view> split(std::string_view line,
                                    const char delimiter);

std::string_view trim_prefix(std::string_view line, std::string_view prefix);
TrimResult trim_prefix(SearchResult &v, std::string_view prefix);

SearchResult get_lines_by_prefix(std::istream &in, std::string_view prefix);

MapStringString get_map_from_line(std::string_view line, const char delimiter);

VectorMapStringString get_vector_map_str_str(const TrimResult &v);
void print(std::ostream &os, const VectorMapStringString &vm);

std::string get_ne_name(std::istream &in, std::string_view prefix,
                        const std::string &name_field);

std::string GetItemByKey(const MapStringString &dict, const std::string &key);

VectorMapStringString Load(const std::string &filename,
                           const std::string &prefix);

std::string LoadNeName(const std::string &filename, const std::string &prefix,
                       const std::string &ne_name);

} // namespace mml
