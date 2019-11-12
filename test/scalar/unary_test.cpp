#include "takatori/scalar/unary.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

class unary_test : public ::testing::Test {};

static_assert(unary::tag == expression_kind::unary);
static_assert(std::is_same_v<type_of_t<unary::tag>, unary>);

TEST_F(unary_test, simple) {
    unary expr {
        unary_operator::sign_inversion,
        constant(1),
    };
    EXPECT_EQ(expr.operator_kind(), unary_operator::sign_inversion);
    EXPECT_EQ(expr.operand(), constant(1));

    EXPECT_EQ(expr.operand().parent_expression().get(), &expr);
}

TEST_F(unary_test, clone) {
    unary expr {
            unary_operator::sign_inversion,
            constant(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(unary_test, clone_move) {
    unary expr {
            unary_operator::sign_inversion,
            constant(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(unary_test, output) {
    unary expr {
            unary_operator::sign_inversion,
            constant(1),
    };

    std::cout << expr << std::endl;
}

TEST_F(unary_test, dispatch_operator) {
    struct cb {
        int operator()(unary_operator_tag_t<unary_operator::plus>, int v) {
            return +v;
        }
        int operator()(unary_operator_tag_t<unary_operator::sign_inversion>, int v) {
            return -v;
        }
        int operator()(unary_operator, int) { return -1; }
    } c;
    EXPECT_EQ(dispatch(c, unary_operator::plus, 1), 1);
    EXPECT_EQ(dispatch(c, unary_operator::sign_inversion, 2), -2);
    EXPECT_EQ(dispatch(c, unary_operator::length, 1), -1);
}

} // namespace takatori::scalar
