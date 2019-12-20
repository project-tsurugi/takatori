#include <takatori/relation/intermediate/distinct.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation::intermediate {

class distinct_relation_test : public ::testing::Test {};

static_assert(distinct::tag == expression_kind::distinct_relation);
static_assert(std::is_same_v<type_of_t<distinct::tag>, distinct>);
static_assert(is_available_in_intermediate_plan(distinct::tag));
static_assert(!is_available_in_step_plan(distinct::tag));

TEST_F(distinct_relation_test, simple) {
    distinct expr {
            vardesc(1),
    };

    ASSERT_EQ(expr.keys().size(), 1);
    EXPECT_EQ(expr.keys()[0], vardesc(1));

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

TEST_F(distinct_relation_test, multiple) {
    distinct expr {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    ASSERT_EQ(expr.keys().size(), 3);
    EXPECT_EQ(expr.keys()[0], vardesc(1));
    EXPECT_EQ(expr.keys()[1], vardesc(2));
    EXPECT_EQ(expr.keys()[2], vardesc(3));
}

TEST_F(distinct_relation_test, clone) {
    distinct expr {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(distinct_relation_test, clone_move) {
    distinct expr {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(distinct_relation_test, output) {
    distinct expr {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
