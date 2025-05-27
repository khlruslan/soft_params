#include <bitset>
#include <cstdint>
#include <gtest/gtest.h>

#include "charconv_util.h"
#include "format_utils.h"
#include "param_compare.h"
#include "params.h"
#include "tabulator.h"

using namespace std::string_literals;
using namespace std::literals::string_view_literals;

namespace my {
namespace project {
namespace {

TEST(FormatTests, ZeroCharsInGroup) {
  std::string test = "0010011";
  auto a = fmt::insert_spaces(test, 0);
  EXPECT_EQ(a, test);
}

TEST(FormatTests, OneBit) {
  std::string test = "0";
  auto a = fmt::insert_spaces(test, 1);
  EXPECT_EQ(a, test);
}

TEST(FormatTests, ZeroBits) {
  std::string test = "";
  auto a = fmt::insert_spaces(test, 4);
  EXPECT_EQ(a, test);
}

TEST(FormatTests, FourBits) {
  std::string test = "0000";
  auto a = fmt::insert_spaces(test, 4);
  EXPECT_EQ(a, test);
}

TEST(FormatTests, FiveBits) {
  std::string test = "11001";
  std::string result = "1 1001";
  auto a = fmt::insert_spaces(test, 4);
  EXPECT_EQ(a, result);
}

TEST(FormatTests, FiveBitsAndTwoCharsInGroup) {
  std::string test = "11001";
  std::string result = "1 10 01";
  auto a = fmt::insert_spaces(test, 2);
  EXPECT_EQ(a, result);
}

TEST(FormatTests, SixBitsAndTwoCharsInGroup) {
  std::string test = "011001";
  std::string result = "01 10 01";
  auto a = fmt::insert_spaces(test, 2);
  EXPECT_EQ(a, result);
}

TEST(FormatTests, FormatLeft) {
  std::string test = "011001";
  std::string result = "011001  ";
  auto a = fmt::format_left(test, 8);
  EXPECT_EQ(a, result);
}

TEST(FormatTests, FormatRight) {
  std::string test = "011001";
  std::string result = "  011001";
  auto a = fmt::format_right(test, 8);
  EXPECT_EQ(a, result);
}

TEST(FormatTests, FormatLeftTooLarge) {
  std::string test = "011001";
  std::string result = "####";
  auto a = fmt::format_left(test, 4);
  EXPECT_EQ(a, result);
}

TEST(FormatTests, FormatRightTooLarge) {
  std::string test = "011001";
  std::string result = "####";
  auto a = fmt::format_right(test, 4);
  EXPECT_EQ(a, result);
}

TEST(FormatTests, FormatLeft32BitAndInsertSpaces) {
  std::bitset<32> source = 255;
  std::string result = "0000 0000 0000 0000 0000 0000 1111 1111 ";
  auto a = fmt::format_left(fmt::insert_spaces(source.to_string(), 4), 40);
  EXPECT_EQ(a, result);
}

TEST(FormatTests, FormatRight32BitAndInsertSpaces) {
  std::bitset<32> source = 0xffff0000ul;
  std::string result = " 1111 1111 1111 1111 0000 0000 0000 0000";
  auto a = fmt::format_right(fmt::insert_spaces(source.to_string(), 4), 40);
  EXPECT_EQ(a, result);
}

TEST(TabulatorTests, GetTopLine) {
  tab::TableSchema desc = {
      {10, "Number"s}, {11, "Value"s}, {39, "Value(bin)"s}};
  auto top_line = tab::GetTopLine(desc);
  std::string result =
      "+----------+-----------+---------------------------------------+"s;

  EXPECT_EQ(top_line, result);
}

TEST(TabulatorTests, GetSepLine) {
  tab::TableSchema desc = {
      {10, "Number"s}, {11, "Value"s}, {39, "Value(bin)"s}};
  auto sep_line = tab::GetRowSeparatorLine(desc);
  std::string result =
      "+----------+-----------+---------------------------------------+"s;

  EXPECT_EQ(sep_line, result);
}

TEST(TabulatorTests, GetFooterLine) {
  tab::TableSchema desc = {
      {10, "Number"s}, {11, "Value"s}, {39, "Value(bin)"s}};
  auto footer_line = tab::GetFooterLine(desc);
  std::string result =
      "+----------+-----------+---------------------------------------+"s;

  EXPECT_EQ(footer_line, result);
}

TEST(TabulatorTests, GetRowLine) {
  tab::TableSchema desc = {
      {10, "Number"s}, {11, "Value"s}, {39, "Value(bin)"s}};
  tab::VectorString row = {{"10"s}, {"234"s}, {"0001 1101"s}};
  auto row_line = tab::GetRowLine(desc, row);

  std::string result =
      "|        10|        234|                              0001 1101|"s;
  EXPECT_EQ(row_line, result);
}

TEST(SVtoInt, Uint8) {
  std::string_view test = "255"sv;
  auto val = util::to_int<uint8_t>(test);

  EXPECT_EQ(255, *val);
}

TEST(SVtoInt, Uint32) {
  std::string_view test = "4294967295"sv;
  auto val = util::to_int<uint32_t>(test);

  EXPECT_EQ(4294967295ul, *val);
}

TEST(BitDifference, Test1) {
  sft::DifferenceInfo d{"BIT_EX_B"s, 5, "0"s, "1"s};
  sft::BitDifference bd(d);

  EXPECT_EQ(bd.IsSignificant(), true);
  auto details = bd.GetDetails();

  sft::DifferenceDetails required = {"BIT_EX_B5"s};

  EXPECT_EQ(details, required);
}

TEST(BitDifference, Test2) {
  sft::DifferenceInfo d{"BIT_EX"s, 5, "0"s, "0"s};
  sft::BitDifference bd(d);

  EXPECT_EQ(bd.IsSignificant(), false);
  auto details = bd.GetDetails();

  EXPECT_EQ(details.empty(), true);
}

TEST(ByteDifference, Test1) {
  sft::DifferenceInfo d{"BYTE_EX_B"s, 5, "255"s, "127"s};
  sft::ByteDifference bd(d);

  EXPECT_EQ(bd.IsSignificant(), true);
  auto details = bd.GetDetails();
  sft::DifferenceDetails required = {"BIT8 of BYTE_EX_B5"s};

  EXPECT_EQ(details, required);
}

TEST(ByteDifference, Test2) {
  sft::DifferenceInfo d{"BYTE_EX"s, 5, "255"s, "63"s};
  sft::ByteDifference bd(d);

  EXPECT_EQ(bd.IsSignificant(), true);
  auto details = bd.GetDetails();
  sft::DifferenceDetails required = {"BIT7 of BYTE_EX5"s, "BIT8 of BYTE_EX5"s};

  EXPECT_EQ(details, required);
}

TEST(DwordDifference, Test1) {
  sft::DifferenceInfo d{"DWORD_EX_B"s, 6, "4294967295"s, "1073741823"s};
  sft::DwordDifference bd(d);

  EXPECT_EQ(bd.IsSignificant(), true);
  auto details = bd.GetDetails();
  sft::DifferenceDetails required = {"BIT31 of DWORD_EX_B6"s,
                                     "BIT32 of DWORD_EX_B6"s};

  EXPECT_EQ(details, required);
}

} // namespace
} // namespace project
} // namespace my

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
