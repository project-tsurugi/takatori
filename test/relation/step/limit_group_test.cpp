#include "takatori/relation/step/limit.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "takatori/util/clonable.h"

namespace takatori::relation::step {

class limit_group_test : public ::testing::Test {};

static_assert(limit::tag == expression_kind::limit_group);
static_assert(std::is_same_v<type_of_t<limit::tag>, limit>);
static_assert(!is_available_in_intermediate_plan(limit::tag));
static_assert(is_available_in_step_plan(limit::tag));

TEST_F(limit_group_test, simple) {
    limit expr { 1 };

    EXPECT_EQ(expr.count(), 1);

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

TEST_F(limit_group_test, clone) {
    limit expr { 2 };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(limit_group_test, clone_move) {
    limit expr { 3 };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(limit_group_test, output) {
    limit expr { 5 };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::step
