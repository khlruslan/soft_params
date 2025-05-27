
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "mml_utils.h"
#include "param_fabric.h"
#include "params.h"

namespace sft {

std::unique_ptr<SoftParameter> CreateBitSoftParameter(std::string_view v) {
  return std::make_unique<BitSoftParameter>(v);
}

std::unique_ptr<SoftParameter> CreateByteSoftParameter(std::string_view v) {
  return std::make_unique<ByteSoftParameter>(v);
}

std::unique_ptr<SoftParameter> CreateDwordSoftParameter(std::string_view v) {
  return std::make_unique<DwordSoftParameter>(v);
}

std::unique_ptr<SoftParameter> CreateStringSoftParameter(std::string_view v) {
  return std::make_unique<StringSoftParameter>(v);
}

std::unique_ptr<SoftParameter>
FabricParameter(const mml::MapStringString &description,
                const std::string &type_name, const FabricMap &fabric_map,
                const std::string &value_field) {
  std::string value;

  if (auto search = description.data_.find(value_field);
      search != description.data_.end()) {
    value = search->second;
  } else {
    return nullptr;
  }

  if (auto it = fabric_map.find(type_name); it != fabric_map.end()) {
    return it->second(value);
  }
  return nullptr;
}

std::unique_ptr<SoftParameter> FabricParameter(const FabricMap &fabric_map,
                                               const ParameterInfo &info) {

  if (auto it = fabric_map.find(info.type_); it != fabric_map.end()) {
    return it->second(info.value_);
  }
  return nullptr;
}

// create differences
std::unique_ptr<IDifference>
CreateBitDifference(const sft::DifferenceInfo &info) {
  return std::make_unique<BitDifference>(info);
}

std::unique_ptr<IDifference>
CreateByteDifference(const sft::DifferenceInfo &info) {
  return std::make_unique<ByteDifference>(info);
}

std::unique_ptr<IDifference>
CreateDwordDifference(const sft::DifferenceInfo &info) {
  return std::make_unique<DwordDifference>(info);
}

std::unique_ptr<IDifference>
CreateStringDifference(const sft::DifferenceInfo &info) {
  return std::make_unique<StringDifference>(info);
}

std::unique_ptr<IDifference>
FabricDifference(const FabricDifferenceMap &fabric_map,
                 const sft::DifferenceInfo &info) {

  if (auto it = fabric_map.find(info.type_); it != fabric_map.end()) {
    return it->second(info);
  }
  return nullptr;
}

} // namespace sft
