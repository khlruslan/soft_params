#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "tabulator.h"

namespace tabulator {

constinit const char chCross = '+';
constinit const char chHLine = '-';
constinit const char chVLine = '|';
constinit const char chSharp = '#';

std::string GetTopLine(const Schema &table) {
  std::string result(1, chCross);
  for (const auto &info : table) {
    result += std::string(info.width_, chHLine) + std::string(1, chCross);
  }
  return result;
}

std::string GetHeaderLine(const Schema &table) {
  std::ostringstream result;
  result << chVLine;
  for (size_t i = 0, is = table.size(); i != is; ++i) {
    result << std::setw(table[i].width_) << table[i].name_ << chVLine;
  }
  return result.str();
}

std::string GetRowSeparatorLine(const Schema &table) {
  return GetTopLine(table);
}

std::string GetRowLine(const Schema &table, const Elements &info) {
  std::ostringstream result;
  if (table.size() == info.size()) {
    result << chVLine;
    for (size_t i = 0, is = table.size(); i != is; ++i) {
      result << table[i].adjust_;
      if (info[i].size() <= table[i].width_) {
        result << std::setw(table[i].width_) << info[i] << chVLine;
      } else {
        result << std::setw(table[i].width_)
               << std::string(table[i].width_, chSharp) << chVLine;
      }
    }
  }
  return result.str();
}

std::string GetFooterLine(const Schema &table) { return GetTopLine(table); }

std::ostream &operator<<(std::ostream &os, Adjust adjust) {
  switch (adjust) {
  case Adjust::Left:
    os << std::left;
    break;
  case Adjust::Right:
    os << std::right;
    break;
  }
  return os;
}

} // namespace tabulator
  //
