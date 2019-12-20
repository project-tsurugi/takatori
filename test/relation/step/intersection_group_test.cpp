#include <takatori/relation/step/intersection.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation::step {

class intersection_group_test : public ::testing::Test {};

static_assert(intersection::tag == expression_kind::intersection_group);
static_assert(std::is_same_v<type_of_t<intersection::tag>, intersection>);
static_assert(!is_available_in_intermediate_plan(intersection::tag));
static_assert(is_available_in_step_plan(intersection::tag));

TEST_F(intersection_group_test, simple) {
    intersection expr;

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

TEST_F(intersection_group_test, clone) {
    intersection expr;

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(intersection_group_test, clone_move) {
    intersection expr;

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(intersection_group_test, output) {
    intersection expr;

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::step
