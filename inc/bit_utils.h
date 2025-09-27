#pragma once

#include <cassert>
#include <climits>
#include <cstdint>
#include <vector>

namespace util {

//  Bits parementers utilities
//
template <typename T> size_t bit_size(T) { return sizeof(T) * CHAR_BIT; }

template <typename T> std::vector<uint16_t> GetDifferentBits(T a, T b) {
  std::vector<uint16_t> result;
  const uint16_t n = static_cast<uint16_t>(bit_size(a));
  assert(n < 65);

  const uint64_t one = 1llu;
  uint64_t lhs = static_cast<uint64_t>(a);
  uint64_t rhs = static_cast<uint64_t>(b);
  uint64_t different_bits = lhs ^ rhs;

  for (uint16_t i = 0; i < n && different_bits; ++i) {
    if (one & different_bits) {
      result.emplace_back(i);
    }
    different_bits >>= 1;
  }

  return result;
}

} // namespace util
