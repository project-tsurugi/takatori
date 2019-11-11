#include "takatori/scalar/match.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

class match_test : public ::testing::Test {};

static_assert(match::tag == expression_kind::match);
static_assert(std::is_same_v<expression_kind_type_t<match::tag>, match>);

TEST_F(match_test, simple) {
    match expr {
            match_operator::like,
            constant(1),
            constant(2),
            constant(3),
    };
    EXPECT_EQ(expr.operator_kind(), match_operator::like);
    EXPECT_EQ(expr.input(), constant(1));
    EXPECT_EQ(expr.pattern(), constant(2));
    EXPECT_EQ(expr.escape(), constant(3));

    EXPECT_EQ(expr.input().parent_expression().get(), &expr);
    EXPECT_EQ(expr.pattern().parent_expression().get(), &expr);
    EXPECT_EQ(expr.escape().parent_expression().get(), &expr);
}

TEST_F(match_test, clone) {
    match expr {
            match_operator::like,
            constant(1),
            constant(2),
            constant(3),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(match_test, clone_move) {
    match expr {
            match_operator::like,
            constant(1),
            constant(2),
            constant(3),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(match_test, output) {
    match expr {
            match_operator::like,
            constant(1),
            constant(2),
            constant(3),
    };

    std::cout << expr << std::endl;
}

TEST_F(match_test, dispatch_operator) {
    struct cb {
        int operator()(match_operator_tag_t<match_operator::like>, int a, int b) {
            return a + b;
        }
        int operator()(match_operator_tag_t<match_operator::similar>, int a, int b) {
            return a - b;
        }
    } c;
    EXPECT_EQ(dispatch(c, match_operator::like, 1, 2), 3);
    EXPECT_EQ(dispatch(c, match_operator::similar, 10, 2), 8);
}

} // namespace takatori::scalar
