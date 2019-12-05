#include "takatori/type/declared.h"

#include <gtest/gtest.h>

#include "takatori/util/clonable.h"
#include "takatori/util/hash.h"

#include "test_utils.h"

namespace takatori::type {

class declared_type_test : public ::testing::Test {};

static_assert(declared::tag == type_kind::declared);
static_assert(std::is_same_v<type_of_t<declared::tag>, declared>);

TEST_F(declared_type_test, simple) {
    declared t { typedesc("A") };
    EXPECT_EQ(t.binding(), typedesc("A"));
}

TEST_F(declared_type_test, hash) {
    using util::hash;
    EXPECT_EQ(hash(declared(typedesc("A"))), hash(declared(typedesc("A"))));
    EXPECT_NE(hash(declared(typedesc("A"))), hash(declared(typedesc("B"))));
}

TEST_F(declared_type_test, clone) {
    declared t { typedesc("A") };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(declared_type_test, clone_move) {
    declared t { typedesc("A") };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(declared_type_test, output) {
    declared t { typedesc("A") };
    std::cout << t << std::endl;
}

} // namespace takatori::type
