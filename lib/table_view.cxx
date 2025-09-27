#include <algorithm>
#include <string>
#include <variant>

#include "soft_param.h"
#include "table_view.h"
#include "tabulator.h"

namespace table {

table::Info GetInfo() {
  using namespace std::string_literals;
  table::Info schema;
  schema.schema_ = {{13, "Name"s, tabulator::Adjust::Left},
                    {11, "Number"s, tabulator::Adjust::Right},
                    {12, "Value"s, tabulator::Adjust::Right},
                    {40, "Value(bin)"s, tabulator::Adjust::Right}};
  schema.top_line_ = tabulator::GetTopLine(schema.schema_);
  schema.header_line_ = tabulator::GetHeaderLine(schema.schema_);
  schema.sep_line_ = tabulator::GetRowSeparatorLine(schema.schema_);
  schema.footer_line_ = tabulator::GetFooterLine(schema.schema_);

  return schema;
}

Rows GetRows(const table::Info &info, const sp::Parameters &params) {
  Rows result;
  result.reserve(params.size());

  tabulator::Elements columns;
  columns.reserve(info.schema_.size());

  std::transform(
      std::begin(params), std::end(params), std::back_inserter(result),
      [&info, &columns](const auto &param) {
        std::visit(
            [&columns](const auto &p) {
              columns.emplace_back(sp::GetName(p));
              columns.emplace_back(std::to_string(sp::GetNumber(p)));
              columns.emplace_back(sp::GetShortView(p));
              columns.emplace_back(sp::GetFullView(p));
            },
            param);
        std::string row = tabulator::GetRowLine(info.schema_, columns);
        columns.resize(0);
        return row;
      });

  return result;
}

Rows GetRows(const table::Info &info, const sp::FoundParameterList &params) {
  Rows result;
  result.reserve(params.size());

  tabulator::Elements columns;
  columns.reserve(info.schema_.size());

  std::transform(
      std::begin(params), std::end(params), std::back_inserter(result),
      [&info, &columns](const auto &param) {
        if (param) {
          std::visit(
              [&columns](const auto &p) {
                columns.emplace_back(sp::GetName(p));
                columns.emplace_back(std::to_string(sp::GetNumber(p)));
                columns.emplace_back(sp::GetShortView(p));
                columns.emplace_back(sp::GetFullView(p));
              },
              *param);
        } else {
          columns.resize(info.schema_.size());
        }
        std::string row = tabulator::GetRowLine(info.schema_, columns);
        columns.resize(0);
        return row;
      });

  return result;
}

void Print(std::ostream &os, const table::Info &info, const Rows &rows) {
  os << info.top_line_ << '\n';
  os << info.header_line_ << '\n';
  os << info.sep_line_ << '\n';
  std::for_each(std::begin(rows), std::end(rows),
                [&os](const auto &row) { os << row << '\n'; });
  os << info.footer_line_ << '\n';
}
} // namespace table
