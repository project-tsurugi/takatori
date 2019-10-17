#include "model/scalar/function_call.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "util/clonable.h"

namespace model::scalar {

class function_call_test : public ::testing::Test {};

static_assert(function_call::tag == expression_kind::function_call);
static_assert(std::is_same_v<expression_kind_type_t<function_call::tag>, function_call>);

TEST_F(function_call_test, simple) {
    function_call expr { function(1) };
    EXPECT_EQ(expr.function(), function(1));

    ASSERT_EQ(expr.arguments().size(), 0);
}

TEST_F(function_call_test, arguments) {
    function_call expr { function(1), { constant(100) } };
    EXPECT_EQ(expr.function(), function(1));

    ASSERT_EQ(expr.arguments().size(), 1);
    ASSERT_EQ(expr.arguments()[0], constant(100));
}

TEST_F(function_call_test, arguments_multiple) {
    function_call expr { function(1), { constant(100), constant(200), constant(300) } };
    EXPECT_EQ(expr.function(), function(1));

    ASSERT_EQ(expr.arguments().size(), 3);
    ASSERT_EQ(expr.arguments()[0], constant(100));
    ASSERT_EQ(expr.arguments()[1], constant(200));
    ASSERT_EQ(expr.arguments()[2], constant(300));
}

TEST_F(function_call_test, clone) {
    function_call expr { function(1), { constant(100), constant(200), constant(300) } };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(function_call_test, output) {
    function_call expr { function(1), { constant(100), constant(200), constant(300) } };
    std::cout << expr << std::endl;
}

} // namespace model::scalar
