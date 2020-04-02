#include <takatori/relation/emit.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation {

class emit_test : public ::testing::Test {};

static_assert(emit::tag == expression_kind::emit);
static_assert(std::is_same_v<type_of_t<emit::tag>, emit>);

TEST_F(emit_test, simple) {
    emit expr {
            vardesc(1),
    };

    EXPECT_EQ(&expr.input().owner(), &expr);
    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), vardesc(1));
        EXPECT_EQ(c.name(), std::nullopt);
    }
    ASSERT_EQ(expr.keys().size(), 0);
    EXPECT_EQ(expr.limit(), std::nullopt);
    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.input());
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 0);
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(emit_test, named_column) {
    emit expr {
            { vardesc(1), "A", },
    };

    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), vardesc(1));
        EXPECT_EQ(c.name(), "A");
    }
}

TEST_F(emit_test, column_multiple) {
    emit expr {
            { vardesc(1), "A", },
            { vardesc(2), "B", },
            { vardesc(3), "C", },
    };

    ASSERT_EQ(expr.columns().size(), 3);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), vardesc(1));
        EXPECT_EQ(c.name(), "A");
    }
    {
        auto&& c = expr.columns()[1];
        EXPECT_EQ(c.source(), vardesc(2));
        EXPECT_EQ(c.name(), "B");
    }
    {
        auto&& c = expr.columns()[2];
        EXPECT_EQ(c.source(), vardesc(3));
        EXPECT_EQ(c.name(), "C");
    }
}

TEST_F(emit_test, key) {
    emit expr {
            {
                    vardesc(1),
            },
            {
                    emit::key { vardesc(10), },
            },
    };

    ASSERT_EQ(expr.columns().size(), 1);
    ASSERT_EQ(expr.keys().size(), 1);
    {
        auto&& k = expr.keys()[0];
        EXPECT_EQ(k.variable(), vardesc(10));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
}

TEST_F(emit_test, key_desc) {
    emit expr {
            {
                    vardesc(1),
            },
            {
                    { vardesc(10), sort_direction::descendant },
            },
    };

    ASSERT_EQ(expr.columns().size(), 1);
    ASSERT_EQ(expr.keys().size(), 1);
    {
        auto&& k = expr.keys()[0];
        EXPECT_EQ(k.variable(), vardesc(10));
        EXPECT_EQ(k.direction(), sort_direction::descendant);
    }
}

TEST_F(emit_test, key_multiple) {
    emit expr {
            {
                    vardesc(1),
            },
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
            },
    };

    ASSERT_EQ(expr.columns().size(), 1);
    ASSERT_EQ(expr.keys().size(), 3);
    {
        auto&& k = expr.keys()[0];
        EXPECT_EQ(k.variable(), vardesc(10));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
    {
        auto&& k = expr.keys()[1];
        EXPECT_EQ(k.variable(), vardesc(20));
        EXPECT_EQ(k.direction(), sort_direction::descendant);
    }
    {
        auto&& k = expr.keys()[2];
        EXPECT_EQ(k.variable(), vardesc(30));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
}

TEST_F(emit_test, limit) {
    emit expr {
            {
                    { vardesc(1), },
            },
            {},
            100,
    };

    EXPECT_EQ(expr.limit(), 100);
}

TEST_F(emit_test, clone) {
    emit expr {
            {
                    { vardesc(1), "A", },
                    { vardesc(2), "B", },
                    vardesc(3),
            },
            {
                    { vardesc(10), sort_direction::ascendant},
                    { vardesc(20), sort_direction::descendant },
                    vardesc(30),
            },
            100,
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(emit_test, clone_move) {
    emit expr {
            {
                    { vardesc(1), "A", },
                    { vardesc(2), "B", },
                    vardesc(3),
            },
            {
                    { vardesc(10), sort_direction::ascendant},
                    { vardesc(20), sort_direction::descendant },
                    vardesc(30),
            },
            100,
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(emit_test, output) {
    emit expr {
            {
                    { vardesc(1), "A", },
                    { vardesc(2), "B", },
                    vardesc(3),
            },
            {
                    { vardesc(10), sort_direction::ascendant},
                    { vardesc(20), sort_direction::descendant },
                    vardesc(30),
            },
            100,
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
