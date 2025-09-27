#pragma once

#include <iostream>
#include <vector>

#include "soft_param.h"
#include "sort_param.h"
#include "tabulator.h"

namespace table {

struct Info {
  tabulator::Schema schema_;
  std::string top_line_;
  std::string header_line_;
  std::string sep_line_;
  std::string footer_line_;
};

table::Info GetInfo();

using Rows = std::vector<std::string>;

Rows GetRows(const table::Info &info, const sp::Parameters &params);
Rows GetRows(const table::Info &info, const sp::FoundParameterList &params);
void Print(std::ostream &os, const table::Info &info, const Rows &rows);

} // namespace table
