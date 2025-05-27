#include <algorithm>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <string>

#include "param_compare.h"
#include "params.h"

namespace sft {

std::ostream &operator<<(std::ostream &os, const KeyTypeId &item) {
  return os << '[' << item.type_ << " ; " << item.id_ << ']';
}

sft::KeysVector CreateIndex(const sft::VectorParameterInfo &v) {
  sft::KeysVector index;
  index.reserve(v.size());
  auto two_fields = [](const ParameterInfo &pi) {
    return sft::KeyTypeId{pi.type_, pi.id_};
  };
  std::ranges::transform(v, std::back_inserter(index), two_fields);
  return index;
}

sft::KeysVector CreateCommonIndex(const sft::VectorParameterInfo &v1,
                                  const sft::VectorParameterInfo &v2) {
  sft::KeysVector index;
  sft::KeysVector i1 = CreateIndex(v1);
  sft::KeysVector i2 = CreateIndex(v2);

  index.reserve(i1.size() + i2.size());
  index.insert(index.end(), i1.begin(), i1.end());
  index.insert(index.end(), i2.begin(), i2.end());

  std::ranges::sort(index);
  auto v_erase = std::ranges::unique(index);
  index.erase(v_erase.begin(), v_erase.end());

  return index;
}

void BitDifference::Init(const DifferenceInfo &info) {
  BitSoftParameter param1(info.value1_);
  BitSoftParameter param2(info.value2_);
  uint8_t diff = param1.GetValue() ^ param2.GetValue();
  if (diff) {
    details_.emplace_back(info.type_ + std::to_string(info.id_));
  }
}

std::vector<int> GetBitsNumbers(uint32_t n, int bits) {
  std::vector<int> result;
  result.reserve(bits);
  for (int i = 0; i < bits && n; ++i, n >>= 1) {
    if (n & 1) {
      result.emplace_back(i + 1);
    }
  }
  return result;
}

void ByteDifference::Init(const DifferenceInfo &info) {
  using namespace std::string_literals;

  ByteSoftParameter param1(info.value1_);
  ByteSoftParameter param2(info.value2_);
  uint8_t diff = param1.GetValue() ^ param2.GetValue();
  if (diff) {
    auto bits = GetBitsNumbers(diff, 8);
    details_.resize(0);
    details_.reserve(bits.size());
    std::string param_name = " of "s + info.type_ + std::to_string(info.id_);
    for (auto b : bits) {
      details_.emplace_back("BIT"s + std::to_string(b) + param_name);
    }
  }
}

void DwordDifference::Init(const DifferenceInfo &info) {
  using namespace std::string_literals;

  DwordSoftParameter param1(info.value1_);
  DwordSoftParameter param2(info.value2_);
  uint32_t diff = param1.GetValue() ^ param2.GetValue();
  if (diff) {
    auto bits = GetBitsNumbers(diff, 32);
    details_.resize(0);
    details_.reserve(bits.size());
    std::string param_name = " of "s + info.type_ + std::to_string(info.id_);
    for (auto b : bits) {
      details_.emplace_back("BIT"s + std::to_string(b) + param_name);
    }
  }
}

void StringDifference::Init(const DifferenceInfo &info) {
  using namespace std::string_literals;

  StringSoftParameter param1(info.value1_);
  StringSoftParameter param2(info.value2_);
  bool diff = param1.GetValue() != param2.GetValue();
  if (diff) {
    details_.resize(0);
    details_ = {"Strings are different."s};
  }
}

} // namespace sft
