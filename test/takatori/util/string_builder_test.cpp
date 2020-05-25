#include <takatori/util/string_builder.h>

#include <gtest/gtest.h>

namespace takatori::util {

class string_builder_test : public ::testing::Test {};

using sb = string_builder;

TEST_F(string_builder_test, chain) {
    auto s = sb {}
            << "hello"
            << 123
            << sb::to_string;
    EXPECT_EQ(s, "hello123");
}

TEST_F(string_builder_test, lvalue) {
    sb ss;
    ss << "hello";
    ss << 123;
    auto s = ss << sb::to_string;
    EXPECT_EQ(s, "hello123");
}

} // namespace takatori::util
