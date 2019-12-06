#include "takatori/relation/step/join.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation::step {

class join_group_test : public ::testing::Test {};

static_assert(join::tag == expression_kind::join_group);
static_assert(std::is_same_v<type_of_t<join::tag>, join>);
static_assert(!is_available_in_intermediate_plan(join::tag));
static_assert(is_available_in_step_plan(join::tag));

TEST_F(join_group_test, simple) {
    join expr {
            join_kind::inner,
            constant(1),
    };

    EXPECT_EQ(expr.operator_kind(), join_kind::inner);
    EXPECT_EQ(expr.condition(), constant(1));
    EXPECT_EQ(expr.condition()->parent_element(), &expr);

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.input());
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(join_group_test, no_extra_condition) {
    join expr {
            join_kind::inner,
    };

    EXPECT_EQ(expr.operator_kind(), join_kind::inner);
    EXPECT_EQ(expr.condition(), nullptr);

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.input());
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
    }
}

TEST_F(join_group_test, clone) {
    join expr {
            join_kind::inner,
            constant(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(join_group_test, clone_move) {
    join expr {
            join_kind::inner,
            constant(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(join_group_test, output) {
    join expr {
            join_kind::inner,
            constant(1),
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::step
