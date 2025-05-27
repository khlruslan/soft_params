#pragma once

#include <charconv>
#include <optional>
#include <stdexcept>
#include <string_view>

namespace util {
template <typename T> auto to_int(std::string_view s) -> std::optional<T> {
  T value{};
#if __cpp_lib_to_chars >= 202306L
  if (std::from_chars(s.data(), s.data() + s.size(), value))
#else
  if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc{})
#endif
    return value;
  else
    return std::nullopt;
}
} // namespace util
