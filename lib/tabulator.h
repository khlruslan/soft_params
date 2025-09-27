#pragma once

#include <string>
#include <vector>

namespace tabulator {

enum class Adjust { Left, Right };

struct ColumnDescr {
  size_t width_;
  std::string name_;
  Adjust adjust_ = Adjust::Right;
};

using Schema = std::vector<ColumnDescr>;
using Elements = std::vector<std::string>;

std::string GetTopLine(const Schema &tabs);
std::string GetHeaderLine(const Schema &tabs);
std::string GetRowSeparatorLine(const Schema &tabs);
std::string GetRowLine(const Schema &tabs, const Elements &info);
std::string GetFooterLine(const Schema &tabs);

std::ostream &operator<<(std::ostream &os, Adjust adjust);

} // namespace tabulator
