#include <takatori/relation/values.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation {

class values_test : public ::testing::Test {};

static_assert(values::tag == expression_kind::values);
static_assert(std::is_same_v<type_of_t<values::tag>, values>);

TEST_F(values_test, simple) {
    values expr {
            {
                    vardesc(1),
            },
            {
                    { constant(1) },
            },
    };

    EXPECT_EQ(&expr.output().owner(), &expr);
    ASSERT_EQ(expr.columns().size(), 1);
    EXPECT_EQ(expr.columns()[0], vardesc(1));
    ASSERT_EQ(expr.rows().size(), 1);
    {
        auto&& r = expr.rows()[0].elements();
        ASSERT_EQ(r.size(), 1);
        EXPECT_EQ(r[0], constant(1));
        EXPECT_EQ(r[0].parent_element(), &expr);
    }

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 0);
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(values_test, multiple) {
    values expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { constant(1), constant(2), constant(3), },
                    { constant(4), constant(5), constant(6), },
                    { constant(7), constant(8), constant(9), },
            },
    };

    ASSERT_EQ(expr.columns().size(), 3);
    EXPECT_EQ(expr.columns()[0], vardesc(1));
    EXPECT_EQ(expr.columns()[1], vardesc(2));
    EXPECT_EQ(expr.columns()[2], vardesc(3));

    ASSERT_EQ(expr.rows().size(), 3);
    {
        auto&& r = expr.rows()[0].elements();
        ASSERT_EQ(r.size(), 3);
        EXPECT_EQ(r[0], constant(1));
        EXPECT_EQ(r[1], constant(2));
        EXPECT_EQ(r[2], constant(3));
    }
    {
        auto&& r = expr.rows()[1].elements();
        ASSERT_EQ(r.size(), 3);
        EXPECT_EQ(r[0], constant(4));
        EXPECT_EQ(r[1], constant(5));
        EXPECT_EQ(r[2], constant(6));
    }
    {
        auto&& r = expr.rows()[2].elements();
        ASSERT_EQ(r.size(), 3);
        EXPECT_EQ(r[0], constant(7));
        EXPECT_EQ(r[1], constant(8));
        EXPECT_EQ(r[2], constant(9));
    }
}

TEST_F(values_test, clone) {
    values expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { constant(1), constant(2), constant(3), },
                    { constant(4), constant(5), constant(6), },
                    { constant(7), constant(8), constant(9), },
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(values_test, clone_move) {
    values expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { constant(1), constant(2), constant(3), },
                    { constant(4), constant(5), constant(6), },
                    { constant(7), constant(8), constant(9), },
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(values_test, output) {
    values expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { constant(1), constant(2), constant(3), },
                    { constant(4), constant(5), constant(6), },
                    { constant(7), constant(8), constant(9), },
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
