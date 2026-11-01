#include <takatori/relation/buffer.h>

#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation {

class buffer_test : public ::testing::Test {};

static_assert(buffer::tag == expression_kind::buffer);
static_assert(std::is_same_v<type_of_t<buffer::tag>, buffer>);

TEST_F(buffer_test, simple) {
    buffer expr { 2 };

    EXPECT_EQ(expr.size(), 2);
    EXPECT_FALSE(expr.stop_with_primary());
    EXPECT_EQ(&expr.primary_output(), &expr.output_ports()[0]);
    {
        auto p = expr.secondary_outputs();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output_ports()[1]);
    }
    EXPECT_EQ(&expr.input().owner(), &expr);
    for (auto& o : expr.output_ports()) {
        EXPECT_EQ(&o.owner(), &expr);
    }

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.input());
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 2);
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(buffer_test, stop_with_primary) {
    buffer expr { 2, true };

    EXPECT_TRUE(expr.stop_with_primary());

    expr.stop_with_primary(false);
    EXPECT_FALSE(expr.stop_with_primary());
}

TEST_F(buffer_test, primary_only) {
    buffer expr { 1 };

    EXPECT_EQ(&expr.primary_output(), &expr.output_ports()[0]);
    EXPECT_TRUE(expr.secondary_outputs().empty());
}

TEST_F(buffer_test, no_outputs) {
    EXPECT_THROW(buffer { 0 }, std::invalid_argument);
}

TEST_F(buffer_test, equals) {
    buffer a { 2 };
    buffer b { 2 };
    buffer c { 3 };
    buffer d { 2, true };

    EXPECT_EQ(a, a);
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
}

TEST_F(buffer_test, clone) {
    buffer expr { 2 };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(buffer_test, clone_stop_with_primary) {
    buffer expr { 2, true };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_TRUE(copy->stop_with_primary());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_EQ(*copy, *move);
    EXPECT_TRUE(move->stop_with_primary());
}

TEST_F(buffer_test, clone_move) {
    buffer expr { 2 };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(buffer_test, output) {
    buffer expr { 2 };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
