#include <takatori/value/decimal.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::value {

class decimal_value_test : public ::testing::Test {
public:
    decimal::entity_type parse(std::string const& value) {
        return static_cast<decimal::entity_type>(::decimal::Decimal(value));
    }
};

static_assert(decimal::tag == value_kind::decimal);
static_assert(std::is_same_v<type_of_t<decimal::tag>, decimal>);

TEST_F(decimal_value_test, simple) {
    decimal v { "3.141592" };
    EXPECT_EQ(v.get(), decimal::entity_type { "3.141592" });
}

TEST_F(decimal_value_test, hash) {
    EXPECT_EQ(std::hash<decimal>{}(decimal("1")), std::hash<decimal>{}(decimal("1")));
    EXPECT_EQ(std::hash<decimal>{}(decimal("2")), std::hash<decimal>{}(decimal("2")));
    EXPECT_NE(std::hash<decimal>{}(decimal("1")), std::hash<decimal>{}(decimal("2")));

    std::hash<decimal> h {};
    EXPECT_EQ(
            h(decimal("3.14")),
            h(decimal("3.14")));
    EXPECT_NE(
            h(decimal("0.1")),
            h(decimal("0.10")));
}

TEST_F(decimal_value_test, clone) {
    decimal v { "2.18" };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());
}

TEST_F(decimal_value_test, clone_move) {
    decimal v { "3.14" };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());

    auto move = util::clone_unique(std::move(v));
    EXPECT_NE(std::addressof(v), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(decimal_value_test, output) {
    decimal v { "123.456" };
    std::cout << v << std::endl;
}

} // namespace takatori::value
