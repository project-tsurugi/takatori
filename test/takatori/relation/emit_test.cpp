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
    EXPECT_EQ(expr.ordering(), emit::ordering_type {});
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

TEST_F(emit_test, ordering) {
    emit expr {
            {
                    { vardesc(1), "A", },
                    { vardesc(2), "B", },
            },
            {
                    emit_ordering::preserve_input,
            },
    };

    ASSERT_EQ(expr.columns().size(), 2);
    EXPECT_EQ(expr.columns()[0].source(), vardesc(1));
    EXPECT_EQ(expr.columns()[1].source(), vardesc(2));
    EXPECT_EQ(expr.ordering(), (emit::ordering_type {
            emit_ordering::preserve_input,
    }));
}

TEST_F(emit_test, ordering_vector) {
    std::vector<emit::column> columns {
            { vardesc(1), "A", },
            { vardesc(2), "B", },
    };
    emit expr {
            std::move(columns),
            {
                    emit_ordering::preserve_input,
            },
    };

    ASSERT_EQ(expr.columns().size(), 2);
    EXPECT_EQ(expr.ordering(), (emit::ordering_type {
            emit_ordering::preserve_input,
    }));
}

TEST_F(emit_test, ordering_setter) {
    emit expr {
            vardesc(1),
    };
    expr.ordering().insert(emit_ordering::preserve_input);
    EXPECT_EQ(expr.ordering(), (emit::ordering_type {
            emit_ordering::preserve_input,
    }));
}

TEST_F(emit_test, equals_ordering) {
    emit a { vardesc(1) };
    emit b { { vardesc(1) }, { emit_ordering::preserve_input } };
    emit c { { vardesc(1) }, { emit_ordering::preserve_input } };
    EXPECT_NE(a, b);
    EXPECT_EQ(b, c);
}

TEST_F(emit_test, clone) {
    emit expr {
            { vardesc(1), "A", },
            { vardesc(2), "B", },
            vardesc(3),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(emit_test, clone_move) {
    emit expr {
            { vardesc(1), "A", },
            { vardesc(2), "B", },
            vardesc(3),
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(emit_test, clone_ordering) {
    emit expr {
            {
                    { vardesc(1), "A", },
            },
            {
                    emit_ordering::preserve_input,
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_EQ(copy->ordering(), expr.ordering());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_EQ(*copy, *move);
    EXPECT_EQ(move->ordering(), copy->ordering());
}

TEST_F(emit_test, output) {
    emit expr {
            { vardesc(1), "A", },
            { vardesc(2), "B", },
            vardesc(3),
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
