#include "takatori/value/character.h"

#include <gtest/gtest.h>

#include "takatori/util/clonable.h"

namespace takatori::value {

class character_value_test : public ::testing::Test {};

static_assert(character::tag == value_kind::character);
static_assert(std::is_same_v<type_of_t<character::tag>, character>);

TEST_F(character_value_test, simple) {
    character v { "Hello, world!" };
    EXPECT_EQ(v.get(), "Hello, world!");
}

TEST_F(character_value_test, hash) {
    EXPECT_EQ(std::hash<character>{}(character("A")), std::hash<character>{}(character("A")));
    EXPECT_EQ(std::hash<character>{}(character("B")), std::hash<character>{}(character("B")));
    EXPECT_NE(std::hash<character>{}(character("A")), std::hash<character>{}(character("B")));
}

TEST_F(character_value_test, clone) {
    character v { "Hello, world!" };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());
}

TEST_F(character_value_test, clone_move) {
    character v { "Hello, world!" };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());

    auto move = util::clone_unique(std::move(v));
    EXPECT_NE(std::addressof(v), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(character_value_test, output) {
    character v { "Hello, world!" };
    std::cout << v << std::endl;
}

} // namespace takatori::value
