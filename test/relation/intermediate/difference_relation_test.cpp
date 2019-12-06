#include "takatori/relation/intermediate/difference.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation::intermediate {

class difference_relation_test : public ::testing::Test {};

static_assert(difference::tag == expression_kind::difference_relation);
static_assert(std::is_same_v<type_of_t<difference::tag>, difference>);
static_assert(is_available_in_intermediate_plan(difference::tag));
static_assert(!is_available_in_step_plan(difference::tag));

TEST_F(difference_relation_test, simple) {
    difference expr {
        { vardesc(1), vardesc(2) }
    };

    EXPECT_EQ(&expr.left().owner(), &expr);
    EXPECT_EQ(&expr.right().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    ASSERT_EQ(expr.key_pairs().size(), 1);
    {
        auto&& p = expr.key_pairs()[0];
        EXPECT_EQ(p.left(), vardesc(1));
        EXPECT_EQ(p.right(), vardesc(2));
    }

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 2);
        EXPECT_EQ(&p[0], &expr.left());
        EXPECT_EQ(&p[1], &expr.right());
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(difference_relation_test, multiple) {
    difference expr {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    };

    ASSERT_EQ(expr.key_pairs().size(), 3);
    {
        auto&& p = expr.key_pairs()[0];
        EXPECT_EQ(p.left(), vardesc(1));
        EXPECT_EQ(p.right(), vardesc(2));
    }
    {
        auto&& p = expr.key_pairs()[1];
        EXPECT_EQ(p.left(), vardesc(3));
        EXPECT_EQ(p.right(), vardesc(4));
    }
    {
        auto&& p = expr.key_pairs()[2];
        EXPECT_EQ(p.left(), vardesc(5));
        EXPECT_EQ(p.right(), vardesc(6));
    }
}

TEST_F(difference_relation_test, clone) {
    difference expr {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(difference_relation_test, clone_move) {
    difference expr {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(difference_relation_test, output) {
    difference expr {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
