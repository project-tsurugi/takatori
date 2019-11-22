#include "takatori/relation/intermediate/join.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation::intermediate {

class join_relation_test : public ::testing::Test {};

static_assert(join::tag == expression_kind::join_relation);
static_assert(std::is_same_v<type_of_t<join::tag>, join>);
static_assert(is_available_in_intermediate_plan(join::tag));
static_assert(!is_available_in_step_plan(join::tag));

TEST_F(join_relation_test, simple) {
    join expr {
        join_kind::full_outer,
        constant(1),
    };

    EXPECT_EQ(&expr.left().owner(), &expr);
    EXPECT_EQ(&expr.right().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    EXPECT_EQ(expr.operator_kind(), join_kind::full_outer);
    EXPECT_EQ(expr.condition(), constant(1));
    EXPECT_EQ(expr.condition()->parent_element(), &expr);

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 2);
        EXPECT_EQ(&p[0], &expr.left());
        EXPECT_EQ(&p[1], &expr.right());
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
    }
}

TEST_F(join_relation_test, cross) {
    join expr {
        join_kind::inner,
    };

    EXPECT_EQ(&expr.left().owner(), &expr);
    EXPECT_EQ(&expr.right().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    EXPECT_EQ(expr.operator_kind(), join_kind::inner);
    EXPECT_FALSE(expr.condition());
}

TEST_F(join_relation_test, clone) {
    join expr {
            join_kind::full_outer,
            constant(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(join_relation_test, clone_move) {
    join expr {
            join_kind::full_outer,
            constant(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(join_relation_test, output) {
    join expr {
            join_kind::full_outer,
            constant(1),
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
