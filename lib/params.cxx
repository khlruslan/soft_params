#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "format_utils.h"
#include "mml_utils.h"
#include "params.h"

namespace sft {

ParameterInfo GetParameterInfo(const mml::MapStringString &description,
                               const mml::ConvertInfo &ci) {
  ParameterInfo param;

  param.type_ = GetItemByKey(description, ci.type_key_);
  std::string value_name = GetItemByKey(ci.type_to_value_, param.type_);
  std::string number_name = GetItemByKey(ci.type_to_number_, param.type_);
  std::string number = GetItemByKey(description, number_name);
  param.id_ = std::stoul(number);
  param.value_ = GetItemByKey(description, value_name);

  return param;
}

VectorParameterInfo Convert(const mml::VectorMapStringString &mml_dict,
                            const mml::ConvertInfo &ci) {
  VectorParameterInfo vpi;
  vpi.reserve(mml_dict.data_.size());

  std::transform(mml_dict.data_.cbegin(), mml_dict.data_.cend(),
                 std::back_inserter(vpi),
                 [&ci](const mml::MapStringString &item) {
                   return GetParameterInfo(item, ci);
                 });

  return vpi;
}

VectorParameterInfo Load(const std::string &file, const std::string &prefix,
                         const mml::ConvertInfo &ci) {

  mml::VectorMapStringString params = mml::Load(file, prefix);

  VectorParameterInfo vpi;
  return Convert(params, ci);
}

std::string BitSoftParameter::GetShortValue() const {
  return std::to_string(value_);
}
std::string BitSoftParameter::GetLongValue() const {
  return std::to_string(value_);
}
std::unique_ptr<SoftParameter> BitSoftParameter::GetEmpty() const {
  return std::make_unique<BitSoftParameter>(0);
}

std::string ByteSoftParameter::GetShortValue() const {
  return std::to_string(value_);
}
std::string ByteSoftParameter::GetLongValue() const {
  return fmt::insert_spaces(std::bitset<8>{value_}.to_string(), 4);
}
std::unique_ptr<SoftParameter> ByteSoftParameter::GetEmpty() const {
  return std::make_unique<ByteSoftParameter>(0);
}

std::string DwordSoftParameter::GetShortValue() const {
  return std::to_string(value_);
}
std::string DwordSoftParameter::GetLongValue() const {
  return fmt::insert_spaces(std::bitset<32>{value_}.to_string(), 4);
}
std::unique_ptr<SoftParameter> DwordSoftParameter::GetEmpty() const {
  return std::make_unique<DwordSoftParameter>(0);
}

std::string StringSoftParameter::GetShortValue() const { return GetData(); }
std::string StringSoftParameter::GetLongValue() const { return GetData(); }
std::unique_ptr<SoftParameter> StringSoftParameter::GetEmpty() const {
  return std::make_unique<StringSoftParameter>(std::string{});
}

} // namespace sft
