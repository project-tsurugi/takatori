#include "takatori/relation/where.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/type/int.h"

#include "takatori/util/clonable.h"

namespace takatori::relation {

class where_test : public ::testing::Test {};

static_assert(where::tag == expression_kind::where);
static_assert(std::is_same_v<type_of_t<where::tag>, where>);

TEST_F(where_test, simple) {
    where expr { constant(1) };

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    EXPECT_EQ(expr.condition(), constant(1));
    EXPECT_EQ(expr.condition().parent_element(), &expr);

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

TEST_F(where_test, clone) {
    where expr { constant(1) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(where_test, clone_move) {
    where expr { constant(1) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(where_test, output) {
    where expr { constant(1) };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
