
#include <algorithm>
#include <iomanip>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>

#include "format_utils.h"

namespace fmt {

std::string insert_spaces(std::string_view sv, size_t n, char sep) {
  std::string result;
  if (n == 0 || sv.size() < 2) {
    result = std::string(sv);
  } else {
    size_t spaces = sv.size() / n;
    spaces = (sv.size() % n == 0 && spaces > 0) ? spaces - 1 : spaces;
    result.reserve(sv.size() + spaces);
    size_t count = 0;

    std::ranges::for_each(std::ranges::reverse_view(sv),
                          [n, sep, &spaces, &count, &result](const auto ch) {
                            result.push_back(ch);
                            ++count;
                            if (count == n) {
                              if (spaces > 0) {
                                result.push_back(sep);
                                --spaces;
                              }
                              count = 0;
                            }
                          });

    std::ranges::reverse(result.begin(), result.end());
  }
  return result;
}

std::string format_left(std::string_view sv, size_t width, char fill_error) {
  std::string result;
  if (width == 0) {
    return result;
  }
  if (sv.size() > width) {
    result = std::string(width, fill_error);
    return result;
  }
  std::ostringstream out;
  out << std::left << std::setw(width) << sv;
  result = std::move(out.str());

  return result;
}

std::string format_right(std::string_view sv, size_t width, char fill_error) {
  std::string result;
  if (width == 0) {
    return result;
  }
  if (sv.size() > width) {
    result = std::string(width, fill_error);
    return result;
  }

  std::ostringstream out;
  out << std::right << std::setw(width) << sv;
  result = std::move(out.str());
  return result;
}

} // namespace fmt
