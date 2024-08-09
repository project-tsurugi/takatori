#include <takatori/value/octet.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::value {

class octet_value_test : public ::testing::Test {};

using std::string_literals::operator""s;

static_assert(octet::tag == value_kind::octet);
static_assert(std::is_same_v<type_of_t<octet::tag>, octet>);

TEST_F(octet_value_test, simple) {
    octet v { "Hello, world!" };
    EXPECT_EQ(v.get(), "Hello, world!");
}

TEST_F(octet_value_test, hash) {
    EXPECT_EQ(std::hash<octet>{}(octet("A")), std::hash<octet>{}(octet("A")));
    EXPECT_EQ(std::hash<octet>{}(octet("B")), std::hash<octet>{}(octet("B")));
    EXPECT_NE(std::hash<octet>{}(octet("A")), std::hash<octet>{}(octet("B")));
}

TEST_F(octet_value_test, clone) {
    octet v { "Hello, world!" };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());
}

TEST_F(octet_value_test, clone_move) {
    octet v { "Hello, world!" };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());

    auto move = util::clone_unique(std::move(v));
    EXPECT_NE(std::addressof(v), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(octet_value_test, output) {
    octet v { "Hello, world!" };
    std::cout << v << std::endl;
}

TEST_F(octet_value_test, parse_octet) {
    std::string output {};
    auto result = parse_octet("01 23 45 67 89 ab cd ef", output);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 8);
    EXPECT_EQ(output, "\x01\x23\x45\x67\x89\xab\xcd\xef"s) << output;
}

TEST_F(octet_value_test, parse_octet_empty) {
    std::string output {};
    auto result = parse_octet("", output);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 0);
    EXPECT_EQ(output, ""s) << output;
}

TEST_F(octet_value_test, parse_octet_append) {
    std::string output { "ABC" };
    auto result = parse_octet(" 58 59 5A ", output);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 3);
    EXPECT_EQ(output, "ABCXYZ"s) << output;
}

TEST_F(octet_value_test, parse_octet_unexpected_char) {
    std::string output {};
    auto result = parse_octet("X", output);
    ASSERT_FALSE(result);
}

TEST_F(octet_value_test, parse_octet_odd) {
    std::string output {};
    auto result = parse_octet("01 23 4", output);
    ASSERT_FALSE(result);
}

} // namespace takatori::value
