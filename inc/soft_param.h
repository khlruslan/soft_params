#pragma once

#include <concepts>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "charconv_util.h"
#include "mml_utils.h"

namespace sp {
enum class Types : uint8_t {
  None,
  Bit,
  Byte,
  Dword,
  String,
  BitEx,
  ByteEx,
  DwordEx,
  StringEx,
  BitExB,
  ByteExB,
  DwordExB,
  StringExB
};

// DiffrenceInfo - list of strings with difference info details

using DifferenceInfo = std::vector<std::string>;

// ParameterInfo
//
struct ParameterInfo {
  std::string type_;
  uint32_t id_ = 0;
  std::string value_;
  auto operator<=>(const ParameterInfo &) const = default;
};

ParameterInfo GetParameterInfo(const mml::MapStringString &description,
                               const mml::ConvertInfo &ci);

using ParameterInfoList = std::vector<ParameterInfo>;

ParameterInfoList Convert(const mml::VectorMapStringString &mml_dict,
                          const mml::ConvertInfo &ci);

ParameterInfoList Load(const std::string &file, const std::string &prefix,
                       const mml::ConvertInfo &ci);

// SoftParameter
struct SoftParameter {
public:
  virtual ~SoftParameter() = default;
  SoftParameter(Types type, int number) : type_(type), number_(number) {}
  SoftParameter() : type_(Types::None), number_(0u) {}
  Types type_ = Types::None; //
  int number_ = 0;
};

template <typename T>
concept DerivedFromSoftParameter = std::derived_from<T, SoftParameter>;

struct BitSoftParameter : public SoftParameter {
  BitSoftParameter(bool value, int number)
      : SoftParameter(Types::Bit, number), value_(value) {}
  bool value_;
};

struct ByteSoftParameter : public SoftParameter {
  ByteSoftParameter(uint8_t value, int number)
      : SoftParameter(Types::Byte, number), value_(value) {}
  uint8_t value_;
};

struct DwordSoftParameter : public SoftParameter {
  DwordSoftParameter(uint32_t value, int number)
      : SoftParameter(Types::Dword, number), value_(value) {}
  uint32_t value_;
};

struct StringSoftParameter : public SoftParameter {
  StringSoftParameter(const std::string &value, int number)
      : SoftParameter(Types::String, number), value_(value) {}
  std::string value_;
};

struct BitExSoftParameter : public SoftParameter {
  BitExSoftParameter(bool value, int number)
      : SoftParameter(Types::BitEx, number), value_(value) {}
  bool value_;
};

struct ByteExSoftParameter : public SoftParameter {
  ByteExSoftParameter(uint8_t value, int number)
      : SoftParameter(Types::ByteEx, number), value_(value) {}
  uint8_t value_;
};

struct DwordExSoftParameter : public SoftParameter {
  DwordExSoftParameter(uint32_t value, int number)
      : SoftParameter(Types::DwordEx, number), value_(value) {}
  uint32_t value_;
};

struct StringExSoftParameter : public SoftParameter {
  StringExSoftParameter(const std::string &value, int number)
      : SoftParameter(Types::StringEx, number), value_(value) {}
  std::string value_;
};

struct BitExBSoftParameter : public SoftParameter {
  BitExBSoftParameter(bool value, int number)
      : SoftParameter(Types::BitExB, number), value_(value) {}
  bool value_;
};

struct ByteExBSoftParameter : public SoftParameter {
  ByteExBSoftParameter(uint8_t value, int number)
      : SoftParameter(Types::ByteExB, number), value_(value) {}
  uint8_t value_;
};

struct DwordExBSoftParameter : public SoftParameter {
  DwordExBSoftParameter(uint32_t value, int number)
      : SoftParameter(Types::DwordExB, number), value_(value) {}
  uint32_t value_;
};

struct StringExBSoftParameter : public SoftParameter {
  StringExBSoftParameter(const std::string &value, int number)
      : SoftParameter(Types::StringExB, number), value_(value) {}
  std::string value_;
};

using Parameter =
    std::variant<sp::BitSoftParameter, sp::ByteSoftParameter,
                 sp::DwordSoftParameter, sp::StringSoftParameter,
                 sp::BitExSoftParameter, sp::ByteExSoftParameter,
                 sp::DwordExSoftParameter, sp::StringExSoftParameter,
                 sp::BitExBSoftParameter, sp::ByteExBSoftParameter,
                 sp::DwordExBSoftParameter, sp::StringExBSoftParameter>;

using Parameters = std::vector<Parameter>;

template <DerivedFromSoftParameter T> Parameter GetParameter(const T &a) {
  return a;
}

template <DerivedFromSoftParameter T> int GetNumber(const T &a) {
  return a.number_;
}

template <DerivedFromSoftParameter T> Types GetType(const T &a) {
  return a.type_;
}

// Create Epmty paramerer
Parameter GetEmptyParameter(const BitSoftParameter &a);

Parameter GetEmptyParameter(const ByteSoftParameter &a);

Parameter GetEmptyParameter(const DwordSoftParameter &a);

Parameter GetEmptyParameter(const StringSoftParameter &a);

Parameter GetEmptyParameter(const BitExSoftParameter &a);

Parameter GetEmptyParameter(const ByteExSoftParameter &a);

Parameter GetEmptyParameter(const DwordExSoftParameter &a);

Parameter GetEmptyParameter(const StringExSoftParameter &a);

Parameter GetEmptyParameter(const BitExBSoftParameter &a);

Parameter GetEmptyParameter(const ByteExBSoftParameter &a);

Parameter GetEmptyParameter(const DwordExBSoftParameter &a);

Parameter GetEmptyParameter(const StringExBSoftParameter &a);

// Get Difference
DifferenceInfo GetDifference(const BitSoftParameter &lhs,
                             const BitSoftParameter &rhs);

DifferenceInfo GetDifference(const ByteSoftParameter &lhs,
                             const ByteSoftParameter &rhs);

DifferenceInfo GetDifference(const DwordSoftParameter &lhs,
                             const DwordSoftParameter &rhs);

DifferenceInfo GetDifference(const StringSoftParameter &lhs,
                             const StringSoftParameter &rhs);

DifferenceInfo GetDifference(const BitExSoftParameter &lhs,
                             const BitExSoftParameter &rhs);

DifferenceInfo GetDifference(const ByteExSoftParameter &lhs,
                             const ByteExSoftParameter &rhs);

DifferenceInfo GetDifference(const DwordExSoftParameter &lhs,
                             const DwordExSoftParameter &rhs);

DifferenceInfo GetDifference(const StringExSoftParameter &lhs,
                             const StringExSoftParameter &rhs);

DifferenceInfo GetDifference(const BitExBSoftParameter &lhs,
                             const BitExBSoftParameter &rhs);

DifferenceInfo GetDifference(const ByteExBSoftParameter &lhs,
                             const ByteExBSoftParameter &rhs);

DifferenceInfo GetDifference(const DwordExBSoftParameter &lhs,
                             const DwordExBSoftParameter &rhs);

DifferenceInfo GetDifference(const StringExBSoftParameter &lhs,
                             const StringExBSoftParameter &rhs);

std::string GetShortView(const BitSoftParameter &p);
std::string GetShortView(const ByteSoftParameter &p);
std::string GetShortView(const DwordSoftParameter &p);
std::string GetShortView(const StringSoftParameter &p);
std::string GetShortView(const BitExSoftParameter &p);
std::string GetShortView(const ByteExSoftParameter &p);
std::string GetShortView(const DwordExSoftParameter &p);
std::string GetShortView(const StringExSoftParameter &p);
std::string GetShortView(const BitExBSoftParameter &p);
std::string GetShortView(const ByteExBSoftParameter &p);
std::string GetShortView(const DwordExBSoftParameter &p);
std::string GetShortView(const StringExBSoftParameter &p);

std::string GetFullView(const BitSoftParameter &p);
std::string GetFullView(const ByteSoftParameter &p);
std::string GetFullView(const DwordSoftParameter &p);
std::string GetFullView(const StringSoftParameter &p);
std::string GetFullView(const BitExSoftParameter &p);
std::string GetFullView(const ByteExSoftParameter &p);
std::string GetFullView(const DwordExSoftParameter &p);
std::string GetFullView(const StringExSoftParameter &p);
std::string GetFullView(const BitExBSoftParameter &p);
std::string GetFullView(const ByteExBSoftParameter &p);
std::string GetFullView(const DwordExBSoftParameter &p);
std::string GetFullView(const StringExBSoftParameter &p);

std::string GetName(const BitSoftParameter &p);
std::string GetName(const ByteSoftParameter &p);
std::string GetName(const DwordSoftParameter &p);
std::string GetName(const StringSoftParameter &p);
std::string GetName(const BitExSoftParameter &p);
std::string GetName(const ByteExSoftParameter &p);
std::string GetName(const DwordExSoftParameter &p);
std::string GetName(const StringExSoftParameter &p);
std::string GetName(const BitExBSoftParameter &p);
std::string GetName(const ByteExBSoftParameter &p);
std::string GetName(const DwordExBSoftParameter &p);
std::string GetName(const StringExBSoftParameter &p);

int GetOrder(const BitSoftParameter &p);
int GetOrder(const ByteSoftParameter &p);
int GetOrder(const DwordSoftParameter &p);
int GetOrder(const StringSoftParameter &p);
int GetOrder(const BitExSoftParameter &p);
int GetOrder(const ByteExSoftParameter &p);
int GetOrder(const DwordExSoftParameter &p);
int GetOrder(const StringExSoftParameter &p);
int GetOrder(const BitExBSoftParameter &p);
int GetOrder(const ByteExBSoftParameter &p);
int GetOrder(const DwordExBSoftParameter &p);
int GetOrder(const StringExBSoftParameter &p);

struct Key {
  int order_;
  int number_;
  auto operator<=>(const Key &) const = default;
};

template <DerivedFromSoftParameter T> Key GetKey(const T &a) {
  return {GetOrder(a), GetNumber(a)};
}

} // namespace sp
