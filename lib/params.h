#pragma once
#include <algorithm>
#include <bitset>
#include <cstdint>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

#include "charconv_util.h"
#include "mml_utils.h"

namespace sft {
struct ParameterInfo {
  std::string type_;
  uint32_t id_ = 0;
  std::string value_;
  auto operator<=>(const ParameterInfo &) const = default;
};

ParameterInfo GetParameterInfo(const mml::MapStringString &description,
                               const mml::ConvertInfo &ci);

using VectorParameterInfo = std::vector<ParameterInfo>;

VectorParameterInfo Convert(const mml::VectorMapStringString &mml_dict,
                            const mml::ConvertInfo &ci);

VectorParameterInfo Load(const std::string &file, const std::string &prefix,
                         const mml::ConvertInfo &ci);

template <std::forward_iterator I, std::sentinel_for<I> S, class T,
          class Proj = std::identity,
          std::indirect_strict_weak_order<const T *, std::projected<I, Proj>>
              Comp = std::ranges::less>
constexpr I binary_find(I first, S last, const T &value, Comp comp = {},
                        Proj proj = {}) {
  first = std::ranges::lower_bound(first, last, value, comp, proj);
  return first != last && !comp(value, proj(*first)) ? first : last;
}

class SoftParameter {
public:
  explicit SoftParameter(std::string_view data) : data_(std::string(data)) {}
  virtual std::string GetShortValue() const = 0;
  virtual std::string GetLongValue() const = 0;
  virtual std::unique_ptr<SoftParameter> GetEmpty() const = 0;
  ~SoftParameter() = default;
  std::string GetData() const { return data_; }

private:
  std::string data_;
};

class BitSoftParameter : public SoftParameter {
public:
  explicit BitSoftParameter(uint8_t value)
      : SoftParameter(std::to_string(value)), value_(value & 0x1u) {}
  explicit BitSoftParameter(std::string_view value) : SoftParameter(value) {
    auto a = util::to_int<uint8_t>(value);
    if (a) {
      value_ = *a & 0x1u;
    }
  }
  std::string GetShortValue() const override;
  std::string GetLongValue() const override;
  std::unique_ptr<SoftParameter> GetEmpty() const override;
  uint8_t GetValue() { return value_; }

private:
  uint8_t value_ = 0;
};

class ByteSoftParameter : public SoftParameter {
public:
  explicit ByteSoftParameter(uint8_t value)
      : SoftParameter(std::to_string(value)), value_(value & 0xffu) {}
  explicit ByteSoftParameter(std::string_view value) : SoftParameter(value) {
    auto a = util::to_int<uint8_t>(value);
    if (a) {
      value_ = *a & 0xffu;
    }
  }
  std::string GetShortValue() const override;
  std::string GetLongValue() const override;
  std::unique_ptr<SoftParameter> GetEmpty() const override;

  uint8_t GetValue() { return value_; }

private:
  uint8_t value_ = 0;
};

class DwordSoftParameter : public SoftParameter {
public:
  explicit DwordSoftParameter(uint32_t value)
      : SoftParameter(std::to_string(value)), value_(value & 0xffu) {}
  explicit DwordSoftParameter(std::string_view value) : SoftParameter(value) {
    auto a = util::to_int<uint32_t>(value);
    if (a) {
      value_ = *a & 0xffffffffu;
    }
  }
  std::string GetShortValue() const override;
  std::string GetLongValue() const override;
  std::unique_ptr<SoftParameter> GetEmpty() const override;

  uint32_t GetValue() { return value_; }

private:
  uint32_t value_ = 0;
};

class StringSoftParameter : public SoftParameter {
public:
  explicit StringSoftParameter(const std::string &value)
      : SoftParameter(value) {}
  explicit StringSoftParameter(std::string_view value) : SoftParameter(value) {}
  std::string GetShortValue() const override;
  std::string GetLongValue() const override;
  std::unique_ptr<SoftParameter> GetEmpty() const override;

  std::string GetValue() { return GetData(); }
};

} // namespace sft
