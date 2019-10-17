#include "model/scalar/coalesce.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "util/clonable.h"

namespace model::scalar {

class coalesce_test : public ::testing::Test {};

static_assert(coalesce::tag == expression_kind::coalesce);
static_assert(std::is_same_v<expression_kind_type_t<coalesce::tag>, coalesce>);

TEST_F(coalesce_test, simple) {
    coalesce expr {
            { constant(1) }
    };
    ASSERT_EQ(expr.alternatives().size(), 1);
    {
        auto&& a = expr.alternatives()[0];
        EXPECT_EQ(a, constant(1));
        EXPECT_EQ(a.parent_expression().get(), &expr);
    }
}

TEST_F(coalesce_test, multiple) {
    coalesce expr {
            {
                    constant(1),
                    constant(2),
                    constant(3),
            }
    };
    ASSERT_EQ(expr.alternatives().size(), 3);
    {
        auto&& a = expr.alternatives()[0];
        EXPECT_EQ(a, constant(1));
        EXPECT_EQ(a.parent_expression().get(), &expr);
    }
    {
        auto&& a = expr.alternatives()[1];
        EXPECT_EQ(a, constant(2));
        EXPECT_EQ(a.parent_expression().get(), &expr);
    }
    {
        auto&& a = expr.alternatives()[2];
        EXPECT_EQ(a, constant(3));
        EXPECT_EQ(a.parent_expression().get(), &expr);
    }
}

TEST_F(coalesce_test, clone) {
    coalesce expr {
            {
                    constant(1),
                    constant(2),
                    constant(3),
            }
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(coalesce_test, clone_move) {
    coalesce expr {
            {
                    constant(1),
                    constant(2),
                    constant(3),
            }
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(coalesce_test, output) {
    coalesce expr {
            {
                    constant(1),
                    constant(2),
                    constant(3),
            }
    };

    std::cout << expr << std::endl;
}

} // namespace model::scalar
