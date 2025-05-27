#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>

#include "mml_utils.h"
#include "param_compare.h"
#include "params.h"

namespace sft {
using FabricPtr = std::unique_ptr<SoftParameter> (*)(std::string_view v);
using FabricMap = std::map<std::string, FabricPtr>;

std::unique_ptr<SoftParameter> CreateBitSoftParameter(std::string_view v);

std::unique_ptr<SoftParameter> CreateByteSoftParameter(std::string_view v);

std::unique_ptr<SoftParameter> CreateDwordSoftParameter(std::string_view v);

std::unique_ptr<SoftParameter> CreateStringSoftParameter(std::string_view v);

std::unique_ptr<SoftParameter>
FabricParameter(const mml::MapStringString &description,
                const std::string &type_field, const FabricMap &fabric_map,
                const std::string &value_field);

std::unique_ptr<SoftParameter> FabricParameter(const FabricMap &fabric_map,
                                               const ParameterInfo &info);

using FabricDifferencePtr =
    std::unique_ptr<IDifference> (*)(const sft::DifferenceInfo &info);
using FabricDifferenceMap = std::map<std::string, FabricDifferencePtr>;
// create differences
std::unique_ptr<IDifference>
CreateBitDifference(const sft::DifferenceInfo &info);
std::unique_ptr<IDifference>
CreateByteDifference(const sft::DifferenceInfo &info);
std::unique_ptr<IDifference>
CreateDwordDifference(const sft::DifferenceInfo &info);
std::unique_ptr<IDifference>
CreateStringDifference(const sft::DifferenceInfo &info);

std::unique_ptr<IDifference>
FabricDifference(const FabricDifferenceMap &fabric_map,
                 const sft::DifferenceInfo &info);

} // namespace sft
