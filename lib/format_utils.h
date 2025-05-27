#pragma once

#include <string>
#include <string_view>

namespace fmt {
constinit const char kSpace = ' ';
constinit const char kSharp = '#';

std::string insert_spaces(std::string_view sv, size_t n, char sep = kSpace);

std::string format_left(std::string_view sv, size_t width = 0,
                        char fill_error_ = kSharp);
std::string format_right(std::string_view sv, size_t width = 0,
                         char fill_error = kSharp);

} // namespace fmt
