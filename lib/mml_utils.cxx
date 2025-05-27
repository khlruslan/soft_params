#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "mml_utils.h"

namespace mml {
const size_t kOne = 1ul;
const char kCharEq = '=';
const char kCharComma = ',';

MapStringString get_map_from_line(std::string_view line, const char delimiter) {
  static std::set<char> chars = {'\n', '\r', ' ', ';'};
  static std::set<char> spaces_brakets = {'\"', '\'', ' '};
  MapStringString result;

  line = trim(line, chars);

  for (auto item : split(line, delimiter)) {
    auto key_value = split(item, kCharEq);
    if (key_value.size() == 2) {
      std::string key = std::string(trim(key_value[0], spaces_brakets));
      std::string value = std::string(trim(key_value[1], spaces_brakets));
      result.data_[key] = value;
    }
  }

  return result;
}

VectorMapStringString get_vector_map_str_str(const TrimResult &v) {
  VectorMapStringString r;
  std::transform(
      v.data_.cbegin(), v.data_.cend(),
      std::insert_iterator(r.data_, r.data_.begin()),
      [](const auto &line) { return get_map_from_line(line, kCharComma); });
  return r;
}

std::vector<std::string_view> split(std::string_view line,
                                    const char delimiter) {
  std::vector<std::string_view> result;

  std::string s_delimiter(1, delimiter);

  for (const auto word : std::views::split(line, s_delimiter)) {
    result.emplace_back(std::string_view(word.begin(), word.end()));
  }

  return result;
}

std::string_view rtrim(std::string_view line, const std::set<char> &chars) {
  if (chars.empty() || line.empty()) {
    return line;
  }

  while (!line.empty() && chars.contains(line.back())) {
    line.remove_suffix(kOne);
  }
  return line;
}

std::string_view ltrim(std::string_view line, const std::set<char> &chars) {
  if (chars.empty() || line.empty()) {
    return line;
  }

  while (!line.empty() && chars.contains(line.front())) {
    line.remove_prefix(kOne);
  }
  return line;
}

std::string_view trim(std::string_view line, const std::set<char> &chars) {
  return rtrim(ltrim(line, chars), chars);
}

std::string_view trim_prefix(std::string_view line, std::string_view prefix) {
  if (line.starts_with(prefix)) {
    line.remove_prefix(prefix.size());
  }
  return line;
}

SearchResult get_lines_by_prefix(std::istream &in, std::string_view prefix) {
  std::string line;
  SearchResult result;
  while (getline(in, line)) {
    if (line.starts_with(prefix)) {
      result.data_.emplace_back(std::move(line));
    }
  }
  return result;
}

TrimResult trim_prefix(SearchResult &v, std::string_view prefix) {
  TrimResult r;
  r.data_.reserve(v.data_.size());
  for (auto it = v.data_.begin(), ite = v.data_.end(); it != ite; ++it) {
    r.data_.emplace_back(std::string(trim_prefix(*it, prefix)));
  }
  return r;
}

void print(std::ostream &os, const VectorMapStringString &vm) {
  for (const auto &m : vm.data_) {
    os << '{' << '\n';
    for (const auto &[key, value] : m.data_) {
      os << "    '" << key << "' : '" << value << "'" << '\n';
    }
    os << '}' << '\n';
  }
}

std::string get_ne_name(std::istream &in, std::string_view prefix,
                        const std::string &name_field) {
  std::string name;
  auto lines = get_lines_by_prefix(in, prefix);
#ifdef my_debug_ne
  std::cerr << __FUNCTION__ << " lines found :" << lines.data_.size() << "\n";
  std::cerr << "           "
            << " prefix :" << prefix << "\n";
#endif
  auto trimmed = trim_prefix(lines, prefix);
  if (!trimmed.data_.empty()) {
    auto command = get_map_from_line(trimmed.data_[0], kCharComma);
    name = GetItemByKey(command, name_field);
#ifdef my_debug_ne
    std::cerr << "             "
              << " name_field :" << name_field << "\n";
    std::cerr << "               "
              << " name :" << name << "\n";
#endif
  }
  return name;
}

std::string GetItemByKey(const mml::MapStringString &dict,
                         const std::string &key) {
  std::string result;
  if (auto it = dict.data_.find(key); it != dict.data_.end()) {
    result = it->second;
  }
  return result;
}

VectorMapStringString Load(const std::string &filename,
                           const std::string &prefix) {
  std::ifstream input(filename);
  auto lines = get_lines_by_prefix(input, prefix);
  input.close();
  auto stripped = mml::trim_prefix(lines, prefix);
  auto params = mml::get_vector_map_str_str(stripped);
  return params;
}

std::string LoadNeName(const std::string &filename, const std::string &prefix,
                       const std::string &ne_field) {
  std::string ne_name;
  std::ifstream in(filename);
  ne_name = mml::get_ne_name(in, prefix, ne_field);
  return ne_name;
}

} // namespace mml
