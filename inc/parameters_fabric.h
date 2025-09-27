#pragma once

#include <stdexcept>

#include "charconv_util.h"
#include "soft_param.h"

namespace sp {

using FuncPtrCreateParameter = Parameter (*)(const ParameterInfo &);

template <Types U>
Parameter CreateParameter(const ParameterInfo &param) = delete;
template <> Parameter CreateParameter<Types::Bit>(const ParameterInfo &param);
template <> Parameter CreateParameter<Types::Byte>(const ParameterInfo &param);
template <> Parameter CreateParameter<Types::Dword>(const ParameterInfo &param);
template <>
Parameter CreateParameter<Types::String>(const ParameterInfo &param);

template <> Parameter CreateParameter<Types::BitEx>(const ParameterInfo &param);
template <>
Parameter CreateParameter<Types::ByteEx>(const ParameterInfo &param);
template <>
Parameter CreateParameter<Types::DwordEx>(const ParameterInfo &param);
template <>
Parameter CreateParameter<Types::StringEx>(const ParameterInfo &param);

template <>
Parameter CreateParameter<Types::BitExB>(const ParameterInfo &param);
template <>
Parameter CreateParameter<Types::ByteExB>(const ParameterInfo &param);
template <>
Parameter CreateParameter<Types::DwordExB>(const ParameterInfo &param);
template <>
Parameter CreateParameter<Types::StringExB>(const ParameterInfo &param);

} // namespace sp
