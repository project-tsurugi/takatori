#include "takatori/relation/intermediate/escape.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation::intermediate {

class escape_test : public ::testing::Test {};

static_assert(escape::tag == expression_kind::escape);
static_assert(std::is_same_v<type_of_t<escape::tag>, escape>);
static_assert(is_available_in_intermediate_plan(escape::tag));
static_assert(!is_available_in_step_plan(escape::tag));

TEST_F(escape_test, simple) {
    escape expr {
        { vardesc(1), vardesc(2) }
    };

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    ASSERT_EQ(expr.mappings().size(), 1);
    {
        auto&& p = expr.mappings()[0];
        EXPECT_EQ(p.source(), vardesc(1));
        EXPECT_EQ(p.destination(), vardesc(2));
    }

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

TEST_F(escape_test, multiple) {
    escape expr {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    };

    ASSERT_EQ(expr.mappings().size(), 3);
    {
        auto&& p = expr.mappings()[0];
        EXPECT_EQ(p.source(), vardesc(1));
        EXPECT_EQ(p.destination(), vardesc(2));
    }
    {
        auto&& p = expr.mappings()[1];
        EXPECT_EQ(p.source(), vardesc(3));
        EXPECT_EQ(p.destination(), vardesc(4));
    }
    {
        auto&& p = expr.mappings()[2];
        EXPECT_EQ(p.source(), vardesc(5));
        EXPECT_EQ(p.destination(), vardesc(6));
    }
}

TEST_F(escape_test, clone) {
    escape expr {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(escape_test, clone_move) {
    escape expr {
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

TEST_F(escape_test, output) {
    escape expr {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
