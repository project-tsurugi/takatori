#include <takatori/scalar/conditional.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::scalar {

class conditional_test : public ::testing::Test {};

static_assert(conditional::tag == expression_kind::conditional);
static_assert(std::is_same_v<type_of_t<conditional::tag>, conditional>);

static_assert(tree::is_tree_fragment_v<conditional::alternative>);

TEST_F(conditional_test, simple) {
    conditional expr {
            {
                    conditional::alternative { constant(1), constant(2) }
            },
    };
    auto&& alts = expr.alternatives();
    ASSERT_EQ(alts.size(), 1);
    {
        auto&& alt = alts[0];
        EXPECT_EQ(alt.condition(), constant(1));
        EXPECT_EQ(alt.body(), constant(2));
        EXPECT_EQ(alt.condition().parent_expression().get(), &expr);
        EXPECT_EQ(alt.body().parent_expression().get(), &expr);
    }

    ASSERT_FALSE(expr.default_expression());
}

TEST_F(conditional_test, default_expression) {
    conditional expr {
            {
                    conditional::alternative { constant(1), constant(2) },
            },
            constant(3),
    };
    auto&& alts = expr.alternatives();
    ASSERT_EQ(alts.size(), 1);
    {
        auto&& alt = alts[0];
        EXPECT_EQ(alt.condition(), constant(1));
        EXPECT_EQ(alt.body(), constant(2));
    }

    ASSERT_TRUE(expr.default_expression());
    EXPECT_EQ(expr.default_expression(), constant(3));
    EXPECT_EQ(expr.default_expression()->parent_expression().get(), &expr);
}

TEST_F(conditional_test, multiple_alternatives) {
    conditional expr {
            {
                    conditional::alternative { constant(1), constant(2) },
                    conditional::alternative { constant(3), constant(4) },
                    conditional::alternative { constant(5), constant(6) },
            },
    };
    auto&& alts = expr.alternatives();
    ASSERT_EQ(alts.size(), 3);
    {
        auto&& alt = alts[0];
        EXPECT_EQ(alt.condition(), constant(1));
        EXPECT_EQ(alt.body(), constant(2));
        EXPECT_EQ(alt.condition().parent_expression().get(), &expr);
        EXPECT_EQ(alt.body().parent_expression().get(), &expr);
    }
    {
        auto&& alt = alts[1];
        EXPECT_EQ(alt.condition(), constant(3));
        EXPECT_EQ(alt.body(), constant(4));
        EXPECT_EQ(alt.condition().parent_expression().get(), &expr);
        EXPECT_EQ(alt.body().parent_expression().get(), &expr);
    }
    {
        auto&& alt = alts[2];
        EXPECT_EQ(alt.condition(), constant(5));
        EXPECT_EQ(alt.body(), constant(6));
        EXPECT_EQ(alt.condition().parent_expression().get(), &expr);
        EXPECT_EQ(alt.body().parent_expression().get(), &expr);
    }

    EXPECT_FALSE(expr.default_expression());
}

TEST_F(conditional_test, alternatives_ownership) {
    conditional expr {
            {
                    conditional::alternative { constant(1), constant(2) },
            },
    };
    ASSERT_EQ(expr.alternatives().size(), 1);

    auto&& element = expr.alternatives()[0];

    auto oc = element.ownership_condition();
    auto ob = element.ownership_body();
    EXPECT_EQ(oc.get(), constant(1));
    EXPECT_EQ(ob.get(), constant(2));

    oc.set(util::clone_unique(constant(100)));
    ob.set(util::clone_unique(constant(200)));
    EXPECT_EQ(oc.get(), constant(100));
    EXPECT_EQ(ob.get(), constant(200));
}

TEST_F(conditional_test, default_expression_ownership) {
    conditional expr {
            {
                    conditional::alternative { constant(1), constant(2) },
            },
            constant(3),
    };
    auto ownership = expr.ownership_default_expression();
    EXPECT_EQ(ownership.get(), constant(3));

    ownership.set(util::clone_unique(constant(100)));
    EXPECT_EQ(ownership.get(), constant(100));
}

TEST_F(conditional_test, clone) {
    conditional expr {
            {
                    conditional::alternative { constant(1), constant(2) },
                    conditional::alternative { constant(3), constant(4) },
                    conditional::alternative { constant(5), constant(6) },
            },
            constant(3),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(conditional_test, output) {
    conditional expr {
            {
                    conditional::alternative { constant(1), constant(2) },
                    conditional::alternative { constant(3), constant(4) },
                    conditional::alternative { constant(5), constant(6) },
            },
            constant(3),
    };
    std::cout << expr << std::endl;
}

} // namespace takatori::scalar
