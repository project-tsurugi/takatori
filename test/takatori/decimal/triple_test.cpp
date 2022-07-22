#include <takatori/decimal/triple.h>

#include <memory>

#include <gtest/gtest.h>

namespace takatori::decimal {

class triple_test : public ::testing::Test {};

TEST_F(triple_test, zero) {
    triple v {};
    EXPECT_EQ(v.sign(), 0);
    EXPECT_EQ(v.coefficient_low(), 0);
    EXPECT_EQ(v.coefficient_high(), 0);
    EXPECT_EQ(v.exponent(), 0);
}

TEST_F(triple_test, simple) {
    triple v { "3.14" };
    EXPECT_EQ(v.sign(), +1);
    EXPECT_EQ(v.coefficient_low(), 314);
    EXPECT_EQ(v.coefficient_high(), 0);
    EXPECT_EQ(v.exponent(), -2);
}

TEST_F(triple_test, output) {
    triple v { "3.1415" };
    std::cout << v << std::endl;
}

} // namespace takatori::decimal
