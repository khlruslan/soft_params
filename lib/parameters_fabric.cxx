#include <sstream>
#include <stdexcept>

#include "charconv_util.h"
#include "parameters_fabric.h"
#include "soft_param.h"

namespace sp {

template <> Parameter CreateParameter<Types::Bit>(const ParameterInfo &param) {
  auto a = util::to_int<uint8_t>(param.value_);
  if (a) {
    return BitSoftParameter(*a & 1u, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for BIT parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return BitSoftParameter(0, param.id_);
}

template <> Parameter CreateParameter<Types::Byte>(const ParameterInfo &param) {
  auto a = util::to_int<uint8_t>(param.value_);
  if (a) {
    return ByteSoftParameter(*a, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for BYTE parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return ByteSoftParameter(0, param.id_);
}

template <>
Parameter CreateParameter<Types::Dword>(const ParameterInfo &param) {
  auto a = util::to_int<uint32_t>(param.value_);
  if (a) {
    return DwordSoftParameter(*a, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for DWORD parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return DwordSoftParameter(0, param.id_);
}

template <>
Parameter CreateParameter<Types::String>(const ParameterInfo &param) {
  return StringSoftParameter(param.value_, param.id_);
}

template <>
Parameter CreateParameter<Types::BitEx>(const ParameterInfo &param) {
  auto a = util::to_int<uint8_t>(param.value_);
  if (a) {
    return BitExSoftParameter(*a & 1u, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for BIT_EX parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return BitExSoftParameter(0, param.id_);
}

template <>
Parameter CreateParameter<Types::ByteEx>(const ParameterInfo &param) {
  auto a = util::to_int<uint8_t>(param.value_);
  if (a) {
    return ByteExSoftParameter(*a, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for BYTE_EX parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return ByteExSoftParameter(0, param.id_);
}

template <>
Parameter CreateParameter<Types::DwordEx>(const ParameterInfo &param) {
  auto a = util::to_int<uint32_t>(param.value_);
  if (a) {
    return DwordExSoftParameter(*a, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for DWORD_EX parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return DwordExSoftParameter(0, param.id_);
}

template <>
Parameter CreateParameter<Types::StringEx>(const ParameterInfo &param) {
  return StringExSoftParameter(param.value_, param.id_);
}

template <>
Parameter CreateParameter<Types::BitExB>(const ParameterInfo &param) {
  auto a = util::to_int<uint8_t>(param.value_);
  if (a) {
    return BitExBSoftParameter(*a & 1u, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for BIT_EX_B parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return BitExBSoftParameter(0, param.id_);
}

template <>
Parameter CreateParameter<Types::ByteExB>(const ParameterInfo &param) {
  auto a = util::to_int<uint8_t>(param.value_);
  if (a) {
    return ByteExBSoftParameter(*a, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for BYTE_EX_B parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return ByteExBSoftParameter(0, param.id_);
}

template <>
Parameter CreateParameter<Types::DwordExB>(const ParameterInfo &param) {
  auto a = util::to_int<uint32_t>(param.value_);
  if (a) {
    return DwordExBSoftParameter(*a, param.id_);
  }
  std::ostringstream ostr;
  ostr << "Wrong string for DWORD_EX_B parameter number :" << param.id_
       << " value:" << param.value_;
  throw(std::runtime_error(ostr.str()));
  return DwordExBSoftParameter(0, param.id_);
}

template <>
Parameter CreateParameter<Types::StringExB>(const ParameterInfo &param) {
  return StringExBSoftParameter(param.value_, param.id_);
}

} // namespace sp
