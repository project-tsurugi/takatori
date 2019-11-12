#include "takatori/scalar/cast.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/type/int.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

class cast_test : public ::testing::Test {};

static_assert(cast::tag == expression_kind::cast);
static_assert(std::is_same_v<type_of_t<cast::tag>, cast>);

TEST_F(cast_test, simple) {
    cast expr { type::int4(), cast::loss_policy_type::ignore, constant(1) };
    EXPECT_EQ(expr.type(), type::int4());
    EXPECT_EQ(expr.loss_policy(), cast::loss_policy_type::ignore);
    EXPECT_EQ(expr.operand(), constant(1));
}

TEST_F(cast_test, clone) {
    cast expr { type::int4(), cast::loss_policy_type::ignore, constant(1) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(cast_test, clone_move) {
    cast expr { type::int4(), cast::loss_policy_type::ignore, constant(1) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(cast_test, output) {
    cast expr { type::int4(), cast::loss_policy_type::ignore, constant(1) };

    std::cout << expr << std::endl;
}

} // namespace takatori::scalar
