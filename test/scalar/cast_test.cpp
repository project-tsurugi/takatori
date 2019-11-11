#include "takatori/scalar/cast.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

class cast_test : public ::testing::Test {};

static_assert(cast::tag == expression_kind::cast);
static_assert(std::is_same_v<expression_kind_type_t<cast::tag>, cast>);

TEST_F(cast_test, simple) {
    cast expr { type(1), cast::loss_policy_type::ignore, constant(2) };
    EXPECT_EQ(expr.type(), type(1));
    EXPECT_EQ(expr.loss_policy(), cast::loss_policy_type::ignore);
    EXPECT_EQ(extract(expr.operand()), constant(2));
}

TEST_F(cast_test, clone) {
    cast expr { type(1), cast::loss_policy_type::ignore, constant(2) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(cast_test, clone_move) {
    cast expr { type(1), cast::loss_policy_type::ignore, constant(2) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(cast_test, output) {
    cast expr { type(1), cast::loss_policy_type::ignore, constant(2) };

    std::cout << expr << std::endl;
}

} // namespace takatori::scalar
