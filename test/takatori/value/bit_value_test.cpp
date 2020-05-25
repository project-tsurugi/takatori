#include <takatori/value/bit.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::value {

class bit_value_test : public ::testing::Test {};

static_assert(bit::tag == value_kind::bit);
static_assert(std::is_same_v<type_of_t<bit::tag>, bit>);

TEST_F(bit_value_test, simple) {
    bit v { "00110101" };
    EXPECT_EQ(v.get(), bit::entity_type(8, 0b00110101));
}

TEST_F(bit_value_test, hash) {
    EXPECT_EQ(std::hash<bit>{}(bit("0")), std::hash<bit>{}(bit("0")));
    EXPECT_EQ(std::hash<bit>{}(bit("0101")), std::hash<bit>{}(bit("0101")));
    EXPECT_NE(std::hash<bit>{}(bit("0101")), std::hash<bit>{}(bit("1010")));
}

TEST_F(bit_value_test, clone) {
    bit v { "00110101" };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());
}

TEST_F(bit_value_test, clone_move) {
    bit v { "00110101" };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());

    auto move = util::clone_unique(std::move(v));
    EXPECT_NE(std::addressof(v), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(bit_value_test, output) {
    bit v { "00110101" };
    std::cout << v << std::endl;
}

} // namespace takatori::value
