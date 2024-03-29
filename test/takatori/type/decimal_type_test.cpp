#include <takatori/type/decimal.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::type {

class decimal_type_test : public ::testing::Test {};

static_assert(decimal::tag == type_kind::decimal);
static_assert(std::is_same_v<type_of_t<decimal::tag>, decimal>);

TEST_F(decimal_type_test, simple) {
    decimal t;
    EXPECT_EQ(t.precision(), std::nullopt);
    EXPECT_EQ(t.scale(), std::nullopt);
}

TEST_F(decimal_type_test, parameters) {
    decimal t { 30, 10 };
    EXPECT_EQ(t.precision(), 30);
    EXPECT_EQ(t.scale(), 10);
}

TEST_F(decimal_type_test, hash) {
    decimal a;
    decimal b { 30, 10 };

    EXPECT_EQ(std::hash<decimal>{}(a), std::hash<decimal>{}(a));
    EXPECT_EQ(std::hash<decimal>{}(b), std::hash<decimal>{}(b));
    EXPECT_NE(std::hash<decimal>{}(a), std::hash<decimal>{}(b));
}

TEST_F(decimal_type_test, clone) {
    decimal t { 30, 10 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(decimal_type_test, clone_move) {
    decimal t { 30, 10 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(decimal_type_test, output) {
    decimal t { 30, 10 };
    std::cout << t << std::endl;
}

TEST_F(decimal_type_test, output_flexible) {
    decimal t {};
    std::cout << t << std::endl;
}

} // namespace takatori::type
