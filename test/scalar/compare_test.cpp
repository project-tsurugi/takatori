#include "takatori/scalar/compare.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

class compare_test : public ::testing::Test {};

static_assert(compare::tag == expression_kind::compare);
static_assert(std::is_same_v<expression_kind_type_t<compare::tag>, compare>);

TEST_F(compare_test, simple) {
    compare expr {
        comparison_operator::equal,
        constant(1),
        constant(2),
    };
    EXPECT_EQ(expr.operator_kind(), comparison_operator::equal);
    EXPECT_EQ(expr.left(), constant(1));
    EXPECT_EQ(expr.right(), constant(2));

    EXPECT_EQ(expr.left().parent_expression().get(), &expr);
    EXPECT_EQ(expr.right().parent_expression().get(), &expr);
}

TEST_F(compare_test, clone) {
    compare expr {
            comparison_operator::equal,
            constant(1),
            constant(2),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(compare_test, clone_move) {
    compare expr {
            comparison_operator::equal,
            constant(1),
            constant(2),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(compare_test, output) {
    compare expr {
            comparison_operator::equal,
            constant(1),
            constant(2),
    };

    std::cout << expr << std::endl;
}

TEST_F(compare_test, dispatch_operator) {
    struct cb {
        bool operator()(comparison_operator_tag_t<comparison_operator::equal>, int a, int b) {
            return a == b;
        }
        bool operator()(comparison_operator_tag_t<comparison_operator::not_equal>, int a, int b) {
            return a != b;
        }
        bool operator()(comparison_operator, int, int) { return false; }
    } c;
    EXPECT_EQ(dispatch(c, comparison_operator::equal, 1, 1), true);
    EXPECT_EQ(dispatch(c, comparison_operator::not_equal, 2, 2), false);
    EXPECT_EQ(dispatch(c, comparison_operator::less, 1, 2), false);
}

} // namespace takatori::scalar
