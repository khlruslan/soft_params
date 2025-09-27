#include <bitset>
#include <cstdint>
#include <gtest/gtest.h>

using namespace std::string_literals;
using namespace std::literals::string_view_literals;

namespace my {
namespace project {
namespace {

TEST(SipmleTest, OneEqOne) {
  int test = 1;
  int a = 1;
  EXPECT_EQ(a, test);
}

} // namespace
} // namespace project
} // namespace my

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
