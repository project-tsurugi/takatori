#include "takatori/relation/select.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/type/int.h"

#include "takatori/util/clonable.h"

namespace takatori::relation {

class select_test : public ::testing::Test {};

static_assert(select::tag == expression_kind::select);
static_assert(std::is_same_v<type_of_t<select::tag>, select>);

TEST_F(select_test, simple) {
    select expr {
        select::column {
            vardesc(1),
            constant(100),
        }
    };

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.variable(), vardesc(1));
        EXPECT_EQ(c.value(), constant(100));
        EXPECT_EQ(c.parent_element(), &expr);
    }
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

TEST_F(select_test, multiple) {
    select expr {
            select::column {
                    vardesc(1),
                    constant(100),
            },
            select::column {
                    vardesc(2),
                    constant(200),
            },
            select::column {
                    vardesc(3),
                    constant(300),
            }
    };

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);

    ASSERT_EQ(expr.columns().size(), 3);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.variable(), vardesc(1));
        EXPECT_EQ(c.value(), constant(100));
        EXPECT_EQ(c.parent_element(), &expr);
    }
    {
        auto&& c = expr.columns()[1];
        EXPECT_EQ(c.variable(), vardesc(2));
        EXPECT_EQ(c.value(), constant(200));
        EXPECT_EQ(c.parent_element(), &expr);
    }
    {
        auto&& c = expr.columns()[2];
        EXPECT_EQ(c.variable(), vardesc(3));
        EXPECT_EQ(c.value(), constant(300));
        EXPECT_EQ(c.parent_element(), &expr);
    }
}

TEST_F(select_test, clone) {
    select expr {
            select::column {
                    vardesc(1),
                    constant(100),
            },
            select::column {
                    vardesc(2),
                    constant(200),
            },
            select::column {
                    vardesc(3),
                    constant(300),
            }
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(select_test, clone_move) {
    select expr {
            select::column {
                    vardesc(1),
                    constant(100),
            },
            select::column {
                    vardesc(2),
                    constant(200),
            },
            select::column {
                    vardesc(3),
                    constant(300),
            }
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(select_test, output) {
    select expr {
            select::column {
                    vardesc(1),
                    constant(100),
            },
            select::column {
                    vardesc(2),
                    constant(200),
            },
            select::column {
                    vardesc(3),
                    constant(300),
            }
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
