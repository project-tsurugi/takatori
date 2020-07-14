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

TEST_F(string_builder_test, null) {
    auto s = null_string_builder {}
            << "hello"
            << 123
            << null_string_builder::to_string;
    EXPECT_EQ(s, "");
}

TEST_F(string_builder_test, debug) {
    auto s = debug_string_builder {}
            << "hello"
            << 123
            << null_string_builder::to_string;
#if defined(NDEBUG)
    EXPECT_EQ(s, "");
#else
    EXPECT_EQ(s, "hello123");
#endif
}

} // namespace takatori::util
