#include "takatori/relation/buffer.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "takatori/util/clonable.h"

namespace takatori::relation {

class buffer_test : public ::testing::Test {};

static_assert(buffer::tag == expression_kind::buffer);
static_assert(std::is_same_v<type_of_t<buffer::tag>, buffer>);

TEST_F(buffer_test, simple) {
    buffer expr { 2 };

    EXPECT_EQ(expr.size(), 2);
    EXPECT_EQ(&expr.input().owner(), &expr);
    for (auto& o : expr.output_ports()) {
        EXPECT_EQ(&o.owner(), &expr);
    }

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.input());
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 2);
    }
}

TEST_F(buffer_test, clone) {
    buffer expr { 2 };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
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
