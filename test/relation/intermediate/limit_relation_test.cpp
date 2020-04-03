#include <takatori/relation/intermediate/limit.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation::intermediate {

class limit_relation_test : public ::testing::Test {};

static_assert(limit::tag == expression_kind::limit_relation);
static_assert(std::is_same_v<type_of_t<limit::tag>, limit>);
static_assert(is_available_in_intermediate_plan(limit::tag));
static_assert(!is_available_in_step_plan(limit::tag));

TEST_F(limit_relation_test, simple) {
    limit expr { 10 };

    ASSERT_EQ(expr.count(), 10);
    ASSERT_EQ(expr.group_keys().size(), 0);
    ASSERT_EQ(expr.sort_keys().size(), 0);

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

TEST_F(limit_relation_test, group_keys) {
    limit expr {
            5,
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
    };

    ASSERT_EQ(expr.count(), 5);
    ASSERT_EQ(expr.group_keys().size(), 3);
    EXPECT_EQ(expr.group_keys()[0], vardesc(1));
    EXPECT_EQ(expr.group_keys()[1], vardesc(2));
    EXPECT_EQ(expr.group_keys()[2], vardesc(3));
}

TEST_F(limit_relation_test, sort_keys) {
    limit expr {
            5,
            {},
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
            },
    };

    ASSERT_EQ(expr.count(), 5);
    ASSERT_EQ(expr.sort_keys().size(), 3);
    {
        auto&& k = expr.sort_keys()[0];
        EXPECT_EQ(k.variable(), vardesc(10));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
    {
        auto&& k = expr.sort_keys()[1];
        EXPECT_EQ(k.variable(), vardesc(20));
        EXPECT_EQ(k.direction(), sort_direction::descendant);
    }
    {
        auto&& k = expr.sort_keys()[2];
        EXPECT_EQ(k.variable(), vardesc(30));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
}

TEST_F(limit_relation_test, unlimited) {
    limit expr {
            {},
            {
                    vardesc(1),
            },
    };

    ASSERT_EQ(expr.count(), std::nullopt);
    ASSERT_EQ(expr.group_keys().size(), 1);
    EXPECT_EQ(expr.group_keys()[0], vardesc(1));
}

TEST_F(limit_relation_test, clone) {
    limit expr {
            5,
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
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
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
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
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
