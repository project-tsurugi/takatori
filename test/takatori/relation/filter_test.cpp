#include <takatori/relation/filter.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation {

class filter_test : public ::testing::Test {};

static_assert(filter::tag == expression_kind::filter);
static_assert(std::is_same_v<type_of_t<filter::tag>, filter>);

TEST_F(filter_test, simple) {
    filter expr { constant(1) };

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    EXPECT_EQ(expr.condition(), constant(1));
    EXPECT_EQ(expr.condition().parent_element(), &expr);

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

TEST_F(filter_test, clone) {
    filter expr { constant(1) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(filter_test, clone_move) {
    filter expr { constant(1) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(filter_test, output) {
    filter expr { constant(1) };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
