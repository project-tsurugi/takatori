#include "takatori/scalar/let.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

class let_test : public ::testing::Test {};

static_assert(let::tag == expression_kind::let);
static_assert(std::is_same_v<expression_kind_type_t<let::tag>, let>);

static_assert(tree::is_tree_fragment_v<let_declarator>);

TEST_F(let_test, simple) {
    let expr {
            { variable(1), constant(2) },
            vref(1),
    };

    ASSERT_EQ(expr.variables().size(), 1);
    {
        auto&& v = expr.variables()[0];
        EXPECT_EQ(v.variable(), variable(1));
        EXPECT_EQ(v.value(), constant(2));
        EXPECT_EQ(v.value().parent_expression().get(), &expr);
    }

    EXPECT_EQ(expr.body(), vref(1));
    EXPECT_EQ(expr.body().parent_expression().get(), &expr);
}

TEST_F(let_test, multiple) {
    let expr {
            {
                    let::declarator { variable(1), constant(2) },
                    let::declarator { variable(3), constant(4) },
                    let::declarator { variable(5), constant(6) },
            },
            vref(1),
    };

    ASSERT_EQ(expr.variables().size(), 3);
    {
        auto&& v = expr.variables()[0];
        EXPECT_EQ(v.variable(), variable(1));
        EXPECT_EQ(v.value(), constant(2));
        EXPECT_EQ(v.value().parent_expression().get(), &expr);
    }
    {
        auto&& v = expr.variables()[1];
        EXPECT_EQ(v.variable(), variable(3));
        EXPECT_EQ(v.value(), constant(4));
        EXPECT_EQ(v.value().parent_expression().get(), &expr);
    }
    {
        auto&& v = expr.variables()[2];
        EXPECT_EQ(v.variable(), variable(5));
        EXPECT_EQ(v.value(), constant(6));
        EXPECT_EQ(v.value().parent_expression().get(), &expr);
    }

    EXPECT_EQ(expr.body(), vref(1));
    EXPECT_EQ(expr.body().parent_expression().get(), &expr);
}

TEST_F(let_test, clone) {
    let expr {
            {
                    let::declarator { variable(1), constant(2) },
                    let::declarator { variable(3), constant(4) },
                    let::declarator { variable(5), constant(6) },
            },
            vref(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(let_test, clone_move) {
    let expr {
            {
                    let::declarator { variable(1), constant(2) },
                    let::declarator { variable(3), constant(4) },
                    let::declarator { variable(5), constant(6) },
            },
            vref(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(let_test, output) {
    let expr {
            {
                    let::declarator { variable(1), constant(2) },
                    let::declarator { variable(3), constant(4) },
                    let::declarator { variable(5), constant(6) },
            },
            vref(1),
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::scalar
