#include "takatori/relation/intermediate/limit.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation::intermediate {

class limit_relation_test : public ::testing::Test {};

static_assert(limit::tag == expression_kind::limit_relation);
static_assert(std::is_same_v<type_of_t<limit::tag>, limit>);
static_assert(is_available_in_intermediate_plan(limit::tag));
static_assert(!is_available_in_step_plan(limit::tag));

TEST_F(limit_relation_test, simple) {
    limit expr { 10 };

    ASSERT_EQ(expr.count(), 10);
    ASSERT_EQ(expr.keys().size(), 0);

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

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);
}

TEST_F(limit_relation_test, grouping) {
    limit expr {
            5,
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
    };

    ASSERT_EQ(expr.count(), 5);
    ASSERT_EQ(expr.keys().size(), 3);
    EXPECT_EQ(expr.keys()[0], vardesc(1));
    EXPECT_EQ(expr.keys()[1], vardesc(2));
    EXPECT_EQ(expr.keys()[2], vardesc(3));
}

TEST_F(limit_relation_test, clone) {
    limit expr {
            5,
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(limit_relation_test, clone_move) {
    limit expr {
            5,
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(limit_relation_test, output) {
    limit expr {
            5,
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
