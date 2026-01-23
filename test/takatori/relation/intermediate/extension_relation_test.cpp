#include <takatori/relation/intermediate/extension.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/relation/test_utils.h>

#include <takatori/util/clonable.h>

#include "dummy_extension.h"

namespace takatori::relation::intermediate {

class extension_relation_test : public ::testing::Test {};

static_assert(extension::tag == expression_kind::extension);
static_assert(std::is_same_v<type_of_t<extension::tag>, extension>);
static_assert(is_available_in_intermediate_plan(extension::tag));
static_assert(!is_available_in_step_plan(extension::tag));

TEST_F(extension_relation_test, simple) {
    dummy_extension expr { "Hello, world!" };
    EXPECT_EQ(expr.value(), "Hello, world!");
    EXPECT_EQ(expr.extension_id(), dummy_extension::extension_tag);

    EXPECT_EQ(expr, dummy_extension("Hello, world!"));
    EXPECT_NE(expr, dummy_extension("OTHER"));
}

TEST_F(extension_relation_test, clone) {
    dummy_extension expr { "Hello, world!" };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(extension_relation_test, clone_move) {
    dummy_extension expr { "Hello, world!" };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(extension_relation_test, output) {
    dummy_extension expr { "Hello, world!" };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
