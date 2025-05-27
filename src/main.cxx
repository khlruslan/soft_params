#include <algorithm>
#include <array>
#include <concepts>
#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <source_location>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "format_utils.h"
#include "mml_utils.h"
#include "param_compare.h"
#include "param_fabric.h"
#include "params.h"
#include "soft_param.h"
#include "tabulator.h"

using namespace std::string_literals;

void print_vector(std::ostream &out, const std::vector<std::string> &data) {
  for (const auto &line : data) {
    out << line << '\n';
  }
}
mml::MapStringString GetMapTypeToValue() {
  static mml::MapStringString result = {{{"BIT"s, "BITVALUE"s},
                                         {"BYTE"s, "BYTEVALUE"s},
                                         {"DWORD"s, "DWORDVALUE"s},
                                         {"STRING"s, "STRINGVALUE"s},
                                         {"BIT_EX"s, "BITVALUE"s},
                                         {"BYTE_EX"s, "BYTEVALUE"s},
                                         {"DWORD_EX"s, "DWORDVALUE"s},
                                         {"STRING_EX"s, "STRINGVALUE"s},
                                         {"BIT_EX_B"s, "BITVALUE"s},
                                         {"BYTE_EX_B"s, "BYTEVALUE"s},
                                         {"DWORD_EX_B"s, "DWORDVALUE"s},
                                         {"STRING_EX_B"s, "STRINGVALUE"s}}};
  return result;
}

mml::MapStringString GetMapTypeToNumberName() {
  static mml::MapStringString result = {{{"BIT"s, "BITNUM"s},
                                         {"BYTE"s, "BYTENUM"s},
                                         {"DWORD"s, "DWORDNUM"s},
                                         {"STRING"s, "STRINGNUM"s},
                                         {"BIT_EX"s, "BITNUM"s},
                                         {"BYTE_EX"s, "BYTENUM"s},
                                         {"DWORD_EX"s, "DWORDNUM"s},
                                         {"STRING_EX"s, "STRINGNUM"s},
                                         {"BIT_EX_B"s, "BITNUM"s},
                                         {"BYTE_EX_B"s, "BYTENUM"s},
                                         {"DWORD_EX_B"s, "DWORDNUM"s},
                                         {"STRING_EX_B"s, "STRINGNUM"s}}};
  return result;
}

std::map<std::string, size_t> GetPrintOrderMap() {
  std::map<std::string, size_t> result = {{{"BIT"s, 1},
                                           {"BYTE"s, 2},
                                           {"DWORD"s, 3},
                                           {"STRING"s, 4},
                                           {"BIT_EX"s, 5},
                                           {"BYTE_EX"s, 6},
                                           {"DWORD_EX"s, 7},
                                           {"STRING_EX"s, 8},
                                           {"BIT_EX_B"s, 9},
                                           {"BYTE_EX_B"s, 10},
                                           {"DWORD_EX_B"s, 11},
                                           {"STRING_EX_B"s, 12}}};
  return result;
}

mml::ConvertInfo GetConvertInfo(const std::string &type_key) {
  mml::ConvertInfo result;
  result.type_to_number_ = GetMapTypeToNumberName();
  result.type_to_value_ = GetMapTypeToValue();
  result.type_key_ = type_key;
  return result;
}

sft::FabricMap GetFabricMap() {
  static sft::FabricMap result = {
      {"BIT"s, sft::CreateBitSoftParameter},
      {"BYTE"s, sft::CreateByteSoftParameter},
      {"DWORD"s, sft::CreateDwordSoftParameter},
      {"STRING"s, sft::CreateStringSoftParameter},
      {"BIT_EX"s, sft::CreateBitSoftParameter},
      {"BYTE_EX"s, sft::CreateByteSoftParameter},
      {"DWORD_EX"s, sft::CreateDwordSoftParameter},
      {"STRING_EX"s, sft::CreateStringSoftParameter},
      {"BIT_EX_B"s, sft::CreateBitSoftParameter},
      {"BYTE_EX_B"s, sft::CreateByteSoftParameter},
      {"DWORD_EX_B"s, sft::CreateDwordSoftParameter},
      {"STRING_EX_B"s, sft::CreateStringSoftParameter}};
  return result;
}

sft::FabricDifferenceMap GetFabricDifferenceMap() {
  static sft::FabricDifferenceMap result = {
      {"BIT"s, sft::CreateBitDifference},
      {"BYTE"s, sft::CreateByteDifference},
      {"DWORD"s, sft::CreateDwordDifference},
      {"STRING"s, sft::CreateStringDifference},
      {"BIT_EX"s, sft::CreateBitDifference},
      {"BYTE_EX"s, sft::CreateByteDifference},
      {"DWORD_EX"s, sft::CreateDwordDifference},
      {"STRING_EX"s, sft::CreateStringDifference},
      {"BIT_EX_B"s, sft::CreateBitDifference},
      {"BYTE_EX_B"s, sft::CreateByteDifference},
      {"DWORD_EX_B"s, sft::CreateDwordDifference},
      {"STRING_EX_B"s, sft::CreateStringDifference}};
  return result;
}

my::TableInfo PrepareTableInfo() {
  my::TableInfo info;
  info.desc_ = {{13, "Name"s, tab::Adjust::Left},
                {11, "Number"s, tab::Adjust::Right},
                {12, "Value"s, tab::Adjust::Right},
                {40, "Value(bin)"s, tab::Adjust::Right}};
  info.top_line_ = tab::GetTopLine(info.desc_);
  info.header_line_ = tab::GetHeaderLine(info.desc_);
  info.sep_line_ = tab::GetRowSeparatorLine(info.desc_);
  info.footer_line_ = tab::GetFooterLine(info.desc_);
  return info;
}

std::unique_ptr<sft::SoftParameter>
CreateParameter(std::optional<sft::ParameterInfo> info,
                const sft::FabricMap &parameter_fabric) {
  if (info) {
    return sft::FabricParameter(parameter_fabric, *info);
  }
  return nullptr;
}

std::unique_ptr<sft::IDifference>
CreateDifference(const std::optional<sft::ParameterInfo> info1,
                 const std::optional<sft::ParameterInfo> info2,
                 const sft::FabricDifferenceMap &difference_fabric) {
  sft::DifferenceInfo di;
  if (!(info1 || info2)) {
    return nullptr;
  }
  if (info1) {
    di.type_ = info1->type_;
    di.id_ = info1->id_;
    di.value1_ = info1->value_;
  }
  if (info2) {
    di.type_ = info2->type_;
    di.id_ = info2->id_;
    di.value2_ = info2->value_;
  }
  return sft::FabricDifference(difference_fabric, di);
}

void PrintResults(std::ostream &out, const my::ComparsionResults &cr,
                  const my::TableInfo &ti, const sft::KeyTypeId &type_id) {
  out << "Difference: NE1 : " << cr.ne[0] << " NE2 : " << cr.ne[1] << '\n';

  out << ti.top_line_ << '\n';
  out << ti.header_line_ << '\n';
  out << ti.sep_line_ << '\n';

  tab::VectorString row;

  row.reserve(ti.desc_.size());
  std::ranges::for_each(cr.param_,
                        [&out, &ti, &type_id, &row](const auto &ptr) {
                          if (ptr) {
                            row.emplace_back(type_id.type_);
                            row.emplace_back(std::to_string(type_id.id_));
                            row.emplace_back(ptr->GetShortValue());
                            row.emplace_back(ptr->GetLongValue());
                          } else {
                            row.resize(ti.desc_.size());
                          }
                          auto row_line = tab::GetRowLine(ti.desc_, row);
                          row.resize(0);
                          out << row_line << '\n';
                        });

  out << ti.footer_line_ << '\n';

  if (cr.diff_) {
    std::ranges::for_each(cr.diff_->GetDetails(),
                          [&out](const auto &item) { out << item << '\n'; });
    out << '\n';
  }
}

sft::KeysVector &
SortByPrintOrder(sft::KeysVector &common_index,
                 const std::map<std::string, size_t> &print_order) {

  static const std::source_location location = std::source_location::current();
  auto get_print_order = [&print_order](const sft::KeyTypeId &item) {
    if (auto search = print_order.find(item.type_);
        search != print_order.end()) {
      return std::tie(search->second, item.type_, item.id_);
    } else {
      throw std::invalid_argument(
          location.file_name() + ":"s + location.function_name() +
          " Sort print_order not found for '"s + item.type_ + "'."s);
    }
  };

  std::ranges::sort(common_index, {}, get_print_order);

  return common_index;
}

my::SortedParams LoadParamAndIndex(const std::string &filename,
                                   const mml::ConvertInfo &ci,
                                   std::string &prefix) {
  my::SortedParams result;
  result.data_ = sft::Load(filename, prefix, ci);
  auto two_fields = [](const sft::ParameterInfo &pi) {
    return sft::KeyTypeId{pi.type_, pi.id_};
  };
  std::ranges::sort(result.data_, {}, two_fields);
  return result;
}

void compare_soft_params(const std::string &input1, const std::string &input2) {

  std::string prefix = "SET SOFTPARA:"s;
  std::string type_field = "DT"s;
  mml::ConvertInfo ci = GetConvertInfo(type_field);

  std::array<my::SortedParams, 2> params = {
      LoadParamAndIndex(input1, ci, prefix),
      LoadParamAndIndex(input2, ci, prefix)};

  sft::KeysVector common_index =
      sft::CreateCommonIndex(params[0].data_, params[1].data_);

  my::TableInfo table_info = PrepareTableInfo();

  sft::FabricDifferenceMap fabric_difference = GetFabricDifferenceMap();
  sft::FabricMap fabric_parameter = GetFabricMap();

  std::string ne_name_field{"NM"s};
  std::string sys_prefix{"SET SYS:"s};

  my::ComparsionResults results;
  results.ne = {mml::LoadNeName(input1, sys_prefix, ne_name_field),
                mml::LoadNeName(input2, sys_prefix, ne_name_field)};

  SortByPrintOrder(common_index, GetPrintOrderMap());

  auto two_fields = [](const sft::ParameterInfo &pi) {
    return sft::KeyTypeId{pi.type_, pi.id_};
  };

  std::ranges::for_each(common_index, [&params, &table_info, &fabric_parameter,
                                       &fabric_difference, two_fields,
                                       &results](const auto &key_value) {
    std::array<std::optional<sft::ParameterInfo>, 2> param_info;

    std::ranges::transform(
        params, param_info.begin(),
        [&key_value, two_fields](
            const auto &parameter) -> std::optional<sft::ParameterInfo> {
          if (auto search = sft::binary_find(parameter.data_.begin(),
                                             parameter.data_.end(), key_value,
                                             {}, two_fields);
              search != parameter.data_.end()) {
            return *search;
          }
          return {};
        });

    if (param_info[0] == param_info[1]) {
      return;
    }

    std::ranges::transform(param_info, results.param_.begin(),
                           [&fabric_parameter](const auto &info) {
                             return CreateParameter(info, fabric_parameter);
                           });

    results.diff_ =
        CreateDifference(param_info[0], param_info[1], fabric_difference);
    if (results.diff_) {
      PrintResults(std::cout, results, table_info, key_value);
    }
  });
}

int main(int argc, char *argv[]) {
  int ver_major = 1;
  int ver_minor = 0;
  std::cout << "Soft paremeters comparsion v" << ver_major << "." << ver_minor
            << "\n";

  std::string filename = "example.txt";
  std::string filename2 = "example01.txt";
  if (3 == argc) {
    filename = argv[1];
    filename2 = argv[2];
  }

  try {
    compare_soft_params(filename, filename2);
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
  }
}
