#include <takatori/relation/join_scan.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

#include <takatori/relation/find.h>

namespace takatori::relation {

class join_scan_test : public ::testing::Test {};

static_assert(join_scan::tag == expression_kind::join_scan);
static_assert(std::is_same_v<type_of_t<join_scan::tag>, join_scan>);
static_assert(is_available_in_intermediate_plan(join_scan::tag));
static_assert(is_available_in_step_plan(join_scan::tag));

TEST_F(join_scan_test, simple) {
    join_scan expr {
            join_kind::full_outer,
            tabledesc("T"),
            {
                    join_scan::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {
                    join_scan::key {
                            columndesc("C2"),
                            constant(0),
                    },
                    endpoint_kind::inclusive,
            },
            {
                    join_scan::key {
                            columndesc("C2"),
                            constant(100),
                    },
                    endpoint_kind::exclusive,
            },
            constant(1),
    };

    EXPECT_EQ(expr.operator_kind(), join_kind::full_outer);
    EXPECT_EQ(expr.source(), tabledesc("T"));
    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), columndesc("C1"));
        EXPECT_EQ(c.destination(), vardesc(1));
    }
    {
        auto&& b = expr.lower();
        ASSERT_EQ(b.keys().size(), 1);
        {
            auto&& k = b.keys()[0];
            EXPECT_EQ(k.variable(), columndesc("C2"));
            EXPECT_EQ(k.value(), constant(0));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        EXPECT_EQ(b.kind(), endpoint_kind::inclusive);
    }
    {
        auto&& b = expr.upper();
        ASSERT_EQ(b.keys().size(), 1);
        {
            auto&& k = b.keys()[0];
            EXPECT_EQ(k.variable(), columndesc("C2"));
            EXPECT_EQ(k.value(), constant(100));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        EXPECT_EQ(b.kind(), endpoint_kind::exclusive);
    }

    EXPECT_EQ(expr.condition(), constant(1));
    EXPECT_EQ(expr.condition()->parent_element(), &expr);

    EXPECT_EQ(&expr.left().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);
    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.left());
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(join_scan_test, multiple) {
    join_scan expr {
            join_kind::full_outer,
            tabledesc("T"),
            {
                    join_scan::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
                    join_scan::column {
                            columndesc("C2"),
                            vardesc(2),
                    },
                    join_scan::column {
                            columndesc("C3"),
                            vardesc(3),
                    },
            },
            {
                    {
                            join_scan::key {
                                    columndesc("C4"),
                                    constant(4),
                            },
                            join_scan::key {
                                    columndesc("C5"),
                                    constant(5),
                            },
                            join_scan::key {
                                    columndesc("C6"),
                                    constant(6),
                            },
                    },
                    endpoint_kind::prefixed_exclusive,
            },
            {
                    {
                            join_scan::key {
                                    columndesc("C4"),
                                    constant(4),
                            },
                            join_scan::key {
                                    columndesc("C5"),
                                    constant(5),
                            },
                    },
                    endpoint_kind::prefixed_inclusive,
            },
            constant(1),
    };

    EXPECT_EQ(expr.operator_kind(), join_kind::full_outer);
    EXPECT_EQ(&expr.output().owner(), &expr);
    EXPECT_EQ(expr.source(), tabledesc("T"));
    ASSERT_EQ(expr.columns().size(), 3);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), columndesc("C1"));
        EXPECT_EQ(c.destination(), vardesc(1));
    }
    {
        auto&& c = expr.columns()[1];
        EXPECT_EQ(c.source(), columndesc("C2"));
        EXPECT_EQ(c.destination(), vardesc(2));
    }
    {
        auto&& c = expr.columns()[2];
        EXPECT_EQ(c.source(), columndesc("C3"));
        EXPECT_EQ(c.destination(), vardesc(3));
    }
    {
        auto&& b = expr.lower();
        ASSERT_EQ(b.keys().size(), 3);
        {
            auto&& k = b.keys()[0];
            EXPECT_EQ(k.variable(), columndesc("C4"));
            EXPECT_EQ(k.value(), constant(4));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        {
            auto&& k = b.keys()[1];
            EXPECT_EQ(k.variable(), columndesc("C5"));
            EXPECT_EQ(k.value(), constant(5));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        {
            auto&& k = b.keys()[2];
            EXPECT_EQ(k.variable(), columndesc("C6"));
            EXPECT_EQ(k.value(), constant(6));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        EXPECT_EQ(b.kind(), endpoint_kind::prefixed_exclusive);
    }
    {
        auto&& b = expr.upper();
        ASSERT_EQ(b.keys().size(), 2);
        {
            auto&& k = b.keys()[0];
            EXPECT_EQ(k.variable(), columndesc("C4"));
            EXPECT_EQ(k.value(), constant(4));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        {
            auto&& k = b.keys()[1];
            EXPECT_EQ(k.variable(), columndesc("C5"));
            EXPECT_EQ(k.value(), constant(5));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        EXPECT_EQ(b.kind(), endpoint_kind::prefixed_inclusive);
    }
}

TEST_F(join_scan_test, clone) {
    join_scan expr {
            join_kind::full_outer,
            tabledesc("T"),
            {
                    join_scan::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {
                    join_scan::key {
                            columndesc("C2"),
                            constant(0),
                    },
                    endpoint_kind::inclusive,
            },
            {
                    join_scan::key {
                            columndesc("C2"),
                            constant(100),
                    },
                    endpoint_kind::exclusive,
            },
            constant(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(join_scan_test, clone_move) {
    join_scan expr {
            join_kind::full_outer,
            tabledesc("T"),
            {
                    join_scan::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {
                    join_scan::key {
                            columndesc("C2"),
                            constant(0),
                    },
                    endpoint_kind::inclusive,
            },
            {
                    join_scan::key {
                            columndesc("C2"),
                            constant(100),
                    },
                    endpoint_kind::exclusive,
            },
            constant(1),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(join_scan_test, output) {
    join_scan expr {
            join_kind::full_outer,
            tabledesc("T"),
            {
                    join_scan::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {
                    join_scan::key {
                            columndesc("C2"),
                            constant(0),
                    },
                    endpoint_kind::inclusive,
            },
            {
                    join_scan::key {
                            columndesc("C2"),
                            constant(100),
                    },
                    endpoint_kind::exclusive,
            },
            constant(1),
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
