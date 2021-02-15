#include <takatori/relation/identify.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation {

class identify_test : public ::testing::Test {};

static_assert(identify::tag == expression_kind::identify);
static_assert(std::is_same_v<type_of_t<identify::tag>, identify>);

TEST_F(identify_test, simple) {
    identify expr { vardesc(1), type::row_id { 2 } };

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    EXPECT_EQ(expr.variable(), vardesc(1));
    EXPECT_EQ(expr.type(), type::row_id { 2 });

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

TEST_F(identify_test, clone) {
    identify expr { vardesc(1), type::row_id { 2 } };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(identify_test, clone_move) {
    identify expr { vardesc(1), type::row_id { 2 } };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(identify_test, output) {
    identify expr { vardesc(1), type::row_id { 2 } };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
