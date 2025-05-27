#pragma once

#include <string>
#include <vector>

namespace tab {

enum class Adjust { Left, Right };

struct ColumnDescr {
  size_t width_;
  std::string name_;
  Adjust adjust_ = Adjust::Right;
};

using TableSchema = std::vector<ColumnDescr>;
using VectorString = std::vector<std::string>;

std::string GetTopLine(const TableSchema &table);
std::string GetHeaderLine(const TableSchema &table);
std::string GetRowSeparatorLine(const TableSchema &table);
std::string GetRowLine(const TableSchema &table, const VectorString &info);
std::string GetFooterLine(const TableSchema &table);

std::ostream &operator<<(std::ostream &os, Adjust adjust);

} // namespace tab
