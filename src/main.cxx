#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "mml_utils.h"
#include "parameters_fabric.h"
#include "print.h"
#include "soft_param.h"
#include "table_view.h"

mml::MapStringString GetMapTypeToValue() {
  using namespace std::string_literals;
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
  using namespace std::string_literals;
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

using MapTypeNameToFucCreateParameterPtr =
    std::map<std::string, sp::FuncPtrCreateParameter>;

MapTypeNameToFucCreateParameterPtr GetMapTypeToFuncPtr() {

  using namespace std::string_literals;
  static MapTypeNameToFucCreateParameterPtr result = {
      {"BIT"s, sp::CreateParameter<sp::Types::Bit>},
      {"BYTE"s, sp::CreateParameter<sp::Types::Byte>},
      {"DWORD"s, sp::CreateParameter<sp::Types::Dword>},
      {"STRING"s, sp::CreateParameter<sp::Types::String>},
      {"BIT_EX"s, sp::CreateParameter<sp::Types::BitEx>},
      {"BYTE_EX"s, sp::CreateParameter<sp::Types::ByteEx>},
      {"DWORD_EX"s, sp::CreateParameter<sp::Types::DwordEx>},
      {"STRING_EX"s, sp::CreateParameter<sp::Types::StringEx>},
      {"BIT_EX_B"s, sp::CreateParameter<sp::Types::BitExB>},
      {"BYTE_EX_B"s, sp::CreateParameter<sp::Types::ByteExB>},
      {"DWORD_EX_B"s, sp::CreateParameter<sp::Types::DwordExB>},
      {"STRING_EX_B"s, sp::CreateParameter<sp::Types::StringExB>}};
  return result;
}

sp::Parameters Convert(const sp::ParameterInfoList &pil) {
  using namespace std::string_literals;
  sp::Parameters result;
  result.reserve(pil.size());
  auto map_to_func = GetMapTypeToFuncPtr();
  std::transform(pil.begin(), pil.end(), std::back_inserter(result),
                 [&map_to_func](const auto &a) {
                   if (auto it = map_to_func.find(a.type_);
                       it != map_to_func.end()) {
                     auto func = it->second;
                     return func(a);
                   }
                   std::ostringstream ostr;
                   ostr << "Wrong parameter type:"s << a.type_ << " number:"s
                        << a.id_ << " value:"s << a.value_;
                   throw std::runtime_error(ostr.str());
                 });

  return result;
}

mml::ConvertInfo GetConvertInfo(const std::string &type_key) {
  mml::ConvertInfo result;
  result.type_to_number_ = GetMapTypeToNumberName();
  result.type_to_value_ = GetMapTypeToValue();
  result.type_key_ = type_key;
  return result;
}

sp::Parameters LoadFile(const std::string &filename) {
  using namespace std::string_literals;
  sp::Parameters result;

  std::string prefix = "SET SOFTPARA:"s;
  std::string type_field = "DT"s;

  mml::ConvertInfo ci = GetConvertInfo(type_field);
  sp::ParameterInfoList pil = sp::Load(filename, prefix, ci);

  result = Convert(pil);

  return result;
  ;
}

void PrintComparsionResult(std::ostream &os, const sp::Parameters &v,
                           const sp::Parameters &v2,
                           const std::vector<std::string> &ne_names) {

  sp::IndexByKey index1 = GetIndexByKey(v);
  sp::IndexByKey index2 = GetIndexByKey(v2);
  sp::IndexByKey common_index = GetCommonIndexByKey(index1, index2);

  table::Info table_info = table::GetInfo();

  auto visitor = [&table_info](auto &&a, auto &&b) -> sp::DifferenceInfo {
    using T1 = std::decay_t<decltype(a)>;
    using T2 = std::decay_t<decltype(b)>;

    if constexpr (std::is_same_v<T1, T2>) {
      return sp::GetDifference(a, b);
    } else {
      throw std::runtime_error("Different types in visitor");
      return sp::DifferenceInfo{};
    }
  };

  std::for_each(
      common_index.begin(), common_index.end(),
      [&os, &v, &v2, &index1, &index2, &table_info, &ne_names,
       &visitor](const auto &item) {
        sp::SearchResultList results = {sp::FindByIndex(index1, item.key_),
                                        sp::FindByIndex(index2, item.key_)};
        sp::FoundParameterList parameters_list = {
            sp::GetFoundParameter(v, results[0]),
            sp::GetFoundParameter(v2, results[1])};

        table::Rows rows = GetRows(table_info, parameters_list);

        sp::FillEmptyParameter(parameters_list);
        auto difference =
            std::visit(visitor, *parameters_list[0], *parameters_list[1]);
        if (!difference.empty()) {
          os << "Difference: NE1 : " << ne_names.at(0)
             << " NE2 : " << ne_names.at(1) << '\n';
          Print(os, table_info, rows);
          print::Print(os, difference);
          os << '\n';
        }
      });
}

std::vector<std::string> GetNeNames(const std::string &file1,
                                    const std::string &file2) {
  using namespace std::string_literals;
  static std::string ne_name_field{"NM"s};
  static std::string sys_prefix{"SET SYS:"s};

  return {mml::LoadNeName(file1, sys_prefix, ne_name_field),
          mml::LoadNeName(file2, sys_prefix, ne_name_field)};
}

int main(int argc, char *argv[]) {
  using namespace std::string_literals;
  int ver_major = 2;
  int ver_minor = 0;
  std::cout << "Soft paremeters comparsion v"s << ver_major << "."s << ver_minor
            << '\n';

  std::string filename1 = "example.txt"s;
  std::string filename2 = "example01.txt"s;
  if (3 == argc) {
    filename1 = argv[1];
    filename2 = argv[2];
  }

  auto p1 = LoadFile(filename1);
  auto p2 = LoadFile(filename2);

  PrintComparsionResult(std::cout, p1, p2, GetNeNames(filename1, filename2));
}
