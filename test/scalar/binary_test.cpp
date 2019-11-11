#include "takatori/scalar/binary.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

class binary_test : public ::testing::Test {};

static_assert(binary::tag == expression_kind::binary);
static_assert(std::is_same_v<expression_kind_type_t<binary::tag>, binary>);

TEST_F(binary_test, simple) {
    binary expr {
        binary_operator::add,
        constant(1),
        constant(2),
    };
    EXPECT_EQ(expr.operator_kind(), binary_operator::add);
    EXPECT_EQ(expr.left(), constant(1));
    EXPECT_EQ(expr.right(), constant(2));

    EXPECT_EQ(expr.left().parent_expression().get(), &expr);
    EXPECT_EQ(expr.right().parent_expression().get(), &expr);
}

TEST_F(binary_test, clone) {
    binary expr {
            binary_operator::add,
            constant(1),
            constant(2),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(binary_test, clone_move) {
    binary expr {
            binary_operator::add,
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

TEST_F(binary_test, output) {
    binary expr {
            binary_operator::add,
            constant(1),
            constant(2),
    };

    std::cout << expr << std::endl;
}

TEST_F(binary_test, dispatch_operator) {
    struct cb {
        int operator()(binary_operator_tag_t<binary_operator::add>, int a, int b) {
            return a + b;
        }
        int operator()(binary_operator_tag_t<binary_operator::subtract>, int a, int b) {
            return a - b;
        }
        int operator()(binary_operator, int, int) { return -1; }
    } c;
    EXPECT_EQ(dispatch(c, binary_operator::add, 1, 2), 3);
    EXPECT_EQ(dispatch(c, binary_operator::subtract, 10, 2), 8);
    EXPECT_EQ(dispatch(c, binary_operator::concat, 1, 2), -1);
}

} // namespace takatori::scalar
