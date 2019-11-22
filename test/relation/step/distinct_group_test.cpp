#include "takatori/relation/step/distinct.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "takatori/util/clonable.h"

namespace takatori::relation::step {

class distinct_group_test : public ::testing::Test {};

static_assert(distinct::tag == expression_kind::distinct_group);
static_assert(std::is_same_v<type_of_t<distinct::tag>, distinct>);
static_assert(!is_available_in_intermediate_plan(distinct::tag));
static_assert(is_available_in_step_plan(distinct::tag));

TEST_F(distinct_group_test, simple) {
    distinct expr;

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

TEST_F(distinct_group_test, clone) {
    distinct expr;

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(distinct_group_test, clone_move) {
    distinct expr;

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(distinct_group_test, output) {
    distinct expr;

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::step
