#include "takatori/relation/find.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation {

class find_test : public ::testing::Test {};

static_assert(find::tag == expression_kind::find);
static_assert(std::is_same_v<type_of_t<find::tag>, find>);

TEST_F(find_test, simple) {
    find expr {
        tabledesc("T"),
        {
            find::column {
                columndesc("C1"),
                vardesc(1),
            },
        },
        {
            find::key {
                columndesc("C2"),
                constant(2),
            },
        },
    };

    EXPECT_EQ(&expr.output().owner(), &expr);
    EXPECT_EQ(expr.source(), tabledesc("T"));
    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), columndesc("C1"));
        EXPECT_EQ(c.destination(), vardesc(1));
    }
    ASSERT_EQ(expr.keys().size(), 1);
    {
        auto&& k = expr.keys()[0];
        EXPECT_EQ(k.variable(), columndesc("C2"));
        EXPECT_EQ(k.value(), constant(2));
        EXPECT_EQ(k.value().parent_element(), &expr);
    }

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 0);
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(find_test, multiple) {
    find expr {
            tabledesc("T"),
            {
                    find::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
                    find::column {
                            columndesc("C2"),
                            vardesc(2),
                    },
                    find::column {
                            columndesc("C3"),
                            vardesc(3),
                    },
            },
            {
                    find::key {
                            columndesc("C4"),
                            constant(4),
                    },
                    find::key {
                            columndesc("C5"),
                            constant(5),
                    },
                    find::key {
                            columndesc("C6"),
                            constant(6),
                    },
            },
    };

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
    ASSERT_EQ(expr.keys().size(), 3);
    {
        auto&& k = expr.keys()[0];
        EXPECT_EQ(k.variable(), columndesc("C4"));
        EXPECT_EQ(k.value(), constant(4));
    }
    {
        auto&& k = expr.keys()[1];
        EXPECT_EQ(k.variable(), columndesc("C5"));
        EXPECT_EQ(k.value(), constant(5));
    }
    {
        auto&& k = expr.keys()[2];
        EXPECT_EQ(k.variable(), columndesc("C6"));
        EXPECT_EQ(k.value(), constant(6));
    }
}

TEST_F(find_test, clone) {
    find expr {
            tabledesc("T"),
            {
                    find::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
                    find::column {
                            columndesc("C2"),
                            vardesc(2),
                    },
                    find::column {
                            columndesc("C3"),
                            vardesc(3),
                    },
            },
            {
                    find::key {
                            columndesc("C4"),
                            constant(4),
                    },
                    find::key {
                            columndesc("C5"),
                            constant(5),
                    },
                    find::key {
                            columndesc("C6"),
                            constant(6),
                    },
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(find_test, clone_move) {
    find expr {
            tabledesc("T"),
            {
                    find::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
                    find::column {
                            columndesc("C2"),
                            vardesc(2),
                    },
                    find::column {
                            columndesc("C3"),
                            vardesc(3),
                    },
            },
            {
                    find::key {
                            columndesc("C4"),
                            constant(4),
                    },
                    find::key {
                            columndesc("C5"),
                            constant(5),
                    },
                    find::key {
                            columndesc("C6"),
                            constant(6),
                    },
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(find_test, output) {
    find expr {
            tabledesc("T"),
            {
                    find::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
                    find::column {
                            columndesc("C2"),
                            vardesc(2),
                    },
                    find::column {
                            columndesc("C3"),
                            vardesc(3),
                    },
            },
            {
                    find::key {
                            columndesc("C4"),
                            constant(4),
                    },
                    find::key {
                            columndesc("C5"),
                            constant(5),
                    },
                    find::key {
                            columndesc("C6"),
                            constant(6),
                    },
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
