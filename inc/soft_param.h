#pragma once

#include "tabulator.h"

namespace my {
struct TableInfo {

  tab::TableSchema desc_;
  std::string top_line_;
  std::string header_line_;
  std::string sep_line_;
  std::string footer_line_;
};

struct ComparsionResults {
  std::array<std::unique_ptr<sft::SoftParameter>, 2> param_;
  std::unique_ptr<sft::IDifference> diff_ = nullptr;
  std::array<std::string, 2> ne;
};

struct SortedParams {
  sft::VectorParameterInfo data_;
};

} // namespace my