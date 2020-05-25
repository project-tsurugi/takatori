#include <takatori/relation/intermediate/intersection.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/relation/test_utils.h>

#include <takatori/util/clonable.h>

namespace takatori::relation::intermediate {

class intersecton_relation_test : public ::testing::Test {};

static_assert(intersection::tag == expression_kind::intersection_relation);
static_assert(std::is_same_v<type_of_t<intersection::tag>, intersection>);
static_assert(is_available_in_intermediate_plan(intersection::tag));
static_assert(!is_available_in_step_plan(intersection::tag));

TEST_F(intersecton_relation_test, simple) {
    intersection expr {
        { vardesc(1), vardesc(2) }
    };

    EXPECT_EQ(&expr.left().owner(), &expr);
    EXPECT_EQ(&expr.right().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    EXPECT_EQ(expr.quantifier(), set_quantifier::all);
    ASSERT_EQ(expr.group_key_pairs().size(), 1);
    {
        auto&& p = expr.group_key_pairs()[0];
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

TEST_F(intersecton_relation_test, quantifier) {
    intersection expr {
            {
                    { vardesc(1), vardesc(2) },
            },
            set_quantifier::distinct,
    };
    EXPECT_EQ(expr.quantifier(), set_quantifier::distinct);
}

TEST_F(intersecton_relation_test, multiple) {
    intersection expr {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    };

    ASSERT_EQ(expr.group_key_pairs().size(), 3);
    {
        auto&& p = expr.group_key_pairs()[0];
        EXPECT_EQ(p.left(), vardesc(1));
        EXPECT_EQ(p.right(), vardesc(2));
    }
    {
        auto&& p = expr.group_key_pairs()[1];
        EXPECT_EQ(p.left(), vardesc(3));
        EXPECT_EQ(p.right(), vardesc(4));
    }
    {
        auto&& p = expr.group_key_pairs()[2];
        EXPECT_EQ(p.left(), vardesc(5));
        EXPECT_EQ(p.right(), vardesc(6));
    }
}

TEST_F(intersecton_relation_test, clone) {
    intersection expr {
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
            set_quantifier::distinct,
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(intersecton_relation_test, clone_move) {
    intersection expr {
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
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

TEST_F(intersecton_relation_test, output) {
    intersection expr {
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
            set_quantifier::distinct,
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
