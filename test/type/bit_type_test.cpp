#include <takatori/type/bit.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::type {

class bit_type_test : public ::testing::Test {};

static_assert(bit::tag == type_kind::bit);
static_assert(std::is_same_v<type_of_t<bit::tag>, bit>);

TEST_F(bit_type_test, simple) {
    bit t { 16 };
    EXPECT_FALSE(t.varying());
    EXPECT_EQ(t.length(), 16);
}

TEST_F(bit_type_test, varying) {
    bit t { varying };
    EXPECT_TRUE(t.varying());
    EXPECT_EQ(t.length().value_or(9999), 9999);
}

TEST_F(bit_type_test, not_varying) {
    bit t { ~varying, 100 };
    EXPECT_FALSE(t.varying());
    EXPECT_EQ(t.length(), 100);
}

TEST_F(bit_type_test, parameters) {
    bit t { varying, 4000 };
    EXPECT_TRUE(t.varying());
    EXPECT_EQ(t.length(), 4000);
}

TEST_F(bit_type_test, hash) {
    bit a { 16 };
    bit b { 4000 };
    bit c { varying, 4000 };

    using namespace std;

    EXPECT_EQ(std::hash<bit>{}(a), std::hash<bit>{}(a));
    EXPECT_EQ(std::hash<bit>{}(b), std::hash<bit>{}(b));
    EXPECT_EQ(std::hash<bit>{}(c), std::hash<bit>{}(c));
    EXPECT_NE(std::hash<bit>{}(a), std::hash<bit>{}(b));
    EXPECT_NE(std::hash<bit>{}(b), std::hash<bit>{}(c));
    EXPECT_NE(std::hash<bit>{}(c), std::hash<bit>{}(a));
}

TEST_F(bit_type_test, clone) {
    bit t { varying, 4000 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(bit_type_test, clone_move) {
    bit t { varying, 4000 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(bit_type_test, output) {
    bit t { varying, 4000 };
    std::cout << t << std::endl;
}

} // namespace takatori::type
