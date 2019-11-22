#include "takatori/relation/intermediate/aggregate.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation::intermediate {

class aggregate_relation_test : public ::testing::Test {};

static_assert(aggregate::tag == expression_kind::aggregate_relation);
static_assert(std::is_same_v<type_of_t<aggregate::tag>, aggregate>);
static_assert(is_available_in_intermediate_plan(aggregate::tag));
static_assert(!is_available_in_step_plan(aggregate::tag));

TEST_F(aggregate_relation_test, simple) {
    aggregate expr {
            {
                    vardesc(1),
            },
            {
                    { aggdesc("SUM"), vardesc(1), vardesc(10) },
            },
    };

    ASSERT_EQ(expr.keys().size(), 1);
    EXPECT_EQ(expr.keys()[0], vardesc(1));

    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.function(), aggdesc("SUM"));
        ASSERT_EQ(c.arguments().size(), 1);
        EXPECT_EQ(c.arguments()[0], vardesc(1));
        EXPECT_EQ(c.destination(), vardesc(10));
    }

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

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

TEST_F(aggregate_relation_test, empty_keys) {
    aggregate expr {
            {},
            {
                    { aggdesc("SUM"), vardesc(4), vardesc(40) },
            },
    };

    ASSERT_EQ(expr.keys().size(), 0);

    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.function(), aggdesc("SUM"));
        ASSERT_EQ(c.arguments().size(), 1);
        EXPECT_EQ(c.arguments()[0], vardesc(4));
        EXPECT_EQ(c.destination(), vardesc(40));
    }
}

TEST_F(aggregate_relation_test, multiple_keys) {
    aggregate expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("SUM"), vardesc(4), vardesc(40) },
            },
    };

    ASSERT_EQ(expr.keys().size(), 3);
    EXPECT_EQ(expr.keys()[0], vardesc(1));
    EXPECT_EQ(expr.keys()[1], vardesc(2));
    EXPECT_EQ(expr.keys()[2], vardesc(3));

    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.function(), aggdesc("SUM"));
        ASSERT_EQ(c.arguments().size(), 1);
        EXPECT_EQ(c.arguments()[0], vardesc(4));
        EXPECT_EQ(c.destination(), vardesc(40));
    }
}

TEST_F(aggregate_relation_test, empty_argument) {
    aggregate expr {
            {
                    vardesc(1),
            },
            {
                    { aggdesc("COUNT"), {}, vardesc(2) },
            }
    };

    ASSERT_EQ(expr.keys().size(), 1);
    EXPECT_EQ(expr.keys()[0], vardesc(1));

    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.function(), aggdesc("COUNT"));
        ASSERT_EQ(c.arguments().size(), 0);
        EXPECT_EQ(c.destination(), vardesc(2));
    }
}

TEST_F(aggregate_relation_test, multiple_arguments) {
    aggregate expr {
            {
                    vardesc(1),
            },
            {
                    { aggdesc("COVAR"), { vardesc(1), vardesc(2) }, vardesc(3) },
            }
    };

    ASSERT_EQ(expr.keys().size(), 1);
    EXPECT_EQ(expr.keys()[0], vardesc(1));

    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.function(), aggdesc("COVAR"));
        ASSERT_EQ(c.arguments().size(), 2);
        EXPECT_EQ(c.arguments()[0], vardesc(1));
        EXPECT_EQ(c.arguments()[1], vardesc(2));
        EXPECT_EQ(c.destination(), vardesc(3));
    }
}

TEST_F(aggregate_relation_test, multiple_columns) {
    aggregate expr {
            {
                    vardesc(1),
            },
            {
                    { aggdesc("COUNT"), vardesc(1), vardesc(2) },
                    { aggdesc("MAX"), vardesc(3), vardesc(4) },
                    { aggdesc("MIN"), vardesc(5), vardesc(6) },
            }
    };

    ASSERT_EQ(expr.keys().size(), 1);
    EXPECT_EQ(expr.keys()[0], vardesc(1));

    ASSERT_EQ(expr.columns().size(), 3);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.function(), aggdesc("COUNT"));
        ASSERT_EQ(c.arguments().size(), 1);
        EXPECT_EQ(c.arguments()[0], vardesc(1));
        EXPECT_EQ(c.destination(), vardesc(2));
    }
    {
        auto&& c = expr.columns()[1];
        EXPECT_EQ(c.function(), aggdesc("MAX"));
        ASSERT_EQ(c.arguments().size(), 1);
        EXPECT_EQ(c.arguments()[0], vardesc(3));
        EXPECT_EQ(c.destination(), vardesc(4));
    }
    {
        auto&& c = expr.columns()[2];
        EXPECT_EQ(c.function(), aggdesc("MIN"));
        ASSERT_EQ(c.arguments().size(), 1);
        EXPECT_EQ(c.arguments()[0], vardesc(5));
        EXPECT_EQ(c.destination(), vardesc(6));
    }
}

TEST_F(aggregate_relation_test, clone) {
    aggregate expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("COUNT"), {}, vardesc(2) },
                    { aggdesc("MAX"), vardesc(3), vardesc(4) },
                    { aggdesc("MIN"), vardesc(5), vardesc(6) },
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(aggregate_relation_test, clone_move) {
    aggregate expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("COUNT"), {}, vardesc(2) },
                    { aggdesc("MAX"), vardesc(3), vardesc(4) },
                    { aggdesc("MIN"), vardesc(5), vardesc(6) },
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(aggregate_relation_test, output) {
    aggregate expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("COUNT"), {}, vardesc(2) },
                    { aggdesc("MAX"), vardesc(3), vardesc(4) },
                    { aggdesc("MIN"), vardesc(5), vardesc(6) },
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
