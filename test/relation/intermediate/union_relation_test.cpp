#include <takatori/relation/intermediate/union.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation::intermediate {

class union_relation_test : public ::testing::Test {};

static_assert(union_::tag == expression_kind::union_relation);
static_assert(std::is_same_v<type_of_t<union_::tag>, union_>);
static_assert(is_available_in_intermediate_plan(union_::tag));
static_assert(!is_available_in_step_plan(union_::tag));

TEST_F(union_relation_test, simple) {
    union_ expr {
            { vardesc(1), vardesc(2), vardesc(3) },
    };

    EXPECT_EQ(expr.quantifier(), set_quantifier::all);
    ASSERT_EQ(expr.mappings().size(), 1);
    {
        auto&& m = expr.mappings()[0];
        EXPECT_EQ(m.left(), vardesc(1));
        EXPECT_EQ(m.right(), vardesc(2));
        EXPECT_EQ(m.destination(), vardesc(3));
    }

    EXPECT_EQ(&expr.left().owner(), &expr);
    EXPECT_EQ(&expr.right().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

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

TEST_F(union_relation_test, quantifier) {
    union_ expr {
            {
                    { vardesc(1), vardesc(2), vardesc(3) },
            },
            set_quantifier::distinct,
    };

    EXPECT_EQ(expr.quantifier(), set_quantifier::distinct);
}

TEST_F(union_relation_test, multiple) {
    union_ expr {
            { vardesc(1), vardesc(2), vardesc(3) },
            { vardesc(4), vardesc(5), vardesc(6) },
            { vardesc(7), vardesc(8), vardesc(9) },
    };

    ASSERT_EQ(expr.mappings().size(), 3);
    {
        auto&& m = expr.mappings()[0];
        EXPECT_EQ(m.left(), vardesc(1));
        EXPECT_EQ(m.right(), vardesc(2));
        EXPECT_EQ(m.destination(), vardesc(3));
    }
    {
        auto&& m = expr.mappings()[1];
        EXPECT_EQ(m.left(), vardesc(4));
        EXPECT_EQ(m.right(), vardesc(5));
        EXPECT_EQ(m.destination(), vardesc(6));
    }
    {
        auto&& m = expr.mappings()[2];
        EXPECT_EQ(m.left(), vardesc(7));
        EXPECT_EQ(m.right(), vardesc(8));
        EXPECT_EQ(m.destination(), vardesc(9));
    }
}

TEST_F(union_relation_test, outer) {
    union_ expr {
            { vardesc(1), {}, vardesc(3) },
            { {}, vardesc(5), vardesc(6) },
            { {}, {}, vardesc(9) },
    };

    ASSERT_EQ(expr.mappings().size(), 3);
    {
        auto&& m = expr.mappings()[0];
        EXPECT_EQ(m.left(), vardesc(1));
        EXPECT_EQ(m.right(), std::nullopt);
        EXPECT_EQ(m.destination(), vardesc(3));
    }
    {
        auto&& m = expr.mappings()[1];
        EXPECT_EQ(m.left(), std::nullopt);
        EXPECT_EQ(m.right(), vardesc(5));
        EXPECT_EQ(m.destination(), vardesc(6));
    }
    {
        auto&& m = expr.mappings()[2];
        EXPECT_EQ(m.left(), std::nullopt);
        EXPECT_EQ(m.right(), std::nullopt);
        EXPECT_EQ(m.destination(), vardesc(9));
    }
}

TEST_F(union_relation_test, clone) {
    union_ expr {
            {
                    { vardesc(1), {}, vardesc(3) },
                    { {}, vardesc(5), vardesc(6) },
                    { vardesc(7), vardesc(8), vardesc(9) },
            },
            set_quantifier::distinct,
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(union_relation_test, clone_move) {
    union_ expr {
            {
                    { vardesc(1), {}, vardesc(3) },
                    { {}, vardesc(5), vardesc(6) },
                    { vardesc(7), vardesc(8), vardesc(9) },
            },
            set_quantifier::distinct,
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(union_relation_test, output) {
    union_ expr {
            {
                    { vardesc(1), {}, vardesc(3) },
                    { {}, vardesc(5), vardesc(6) },
                    { vardesc(7), vardesc(8), vardesc(9) },
            },
            set_quantifier::distinct,
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
