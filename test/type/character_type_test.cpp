#include "takatori/type/character.h"

#include <gtest/gtest.h>

#include "takatori/util/clonable.h"

namespace takatori::type {

class character_type_test : public ::testing::Test {};

static_assert(character::tag == type_kind::character);
static_assert(std::is_same_v<type_of_t<character::tag>, character>);

TEST_F(character_type_test, simple) {
    character t { 16 };
    EXPECT_FALSE(t.varying());
    EXPECT_EQ(t.length(), 16);
}

TEST_F(character_type_test, varying) {
    character t { varying };
    EXPECT_TRUE(t.varying());
    EXPECT_EQ(t.length().value_or(9999), 9999);
}

TEST_F(character_type_test, not_varying) {
    character t { ~varying, 100 };
    EXPECT_FALSE(t.varying());
    EXPECT_EQ(t.length(), 100);
}

TEST_F(character_type_test, parameters) {
    character t { varying, 4000 };
    EXPECT_TRUE(t.varying());
    EXPECT_EQ(t.length(), 4000);
}

TEST_F(character_type_test, hash) {
    character a { 16 };
    character b { 4000 };
    character c { varying, 4000 };

    EXPECT_EQ(std::hash<character>{}(a), std::hash<character>{}(a));
    EXPECT_EQ(std::hash<character>{}(b), std::hash<character>{}(b));
    EXPECT_EQ(std::hash<character>{}(c), std::hash<character>{}(c));
    EXPECT_NE(std::hash<character>{}(a), std::hash<character>{}(b));
    EXPECT_NE(std::hash<character>{}(b), std::hash<character>{}(c));
    EXPECT_NE(std::hash<character>{}(c), std::hash<character>{}(a));
}

TEST_F(character_type_test, clone) {
    character t { varying, 4000 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(character_type_test, clone_move) {
    character t { varying, 4000 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(character_type_test, output) {
    character t { varying, 4000 };
    std::cout << t << std::endl;
}

} // namespace takatori::type
