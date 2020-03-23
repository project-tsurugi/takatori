#include <takatori/scalar/variable_reference.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::scalar {

class variable_reference_test : public ::testing::Test {};

static_assert(variable_reference::tag == expression_kind::variable_reference);
static_assert(std::is_same_v<type_of_t<variable_reference::tag>, variable_reference>);

TEST_F(variable_reference_test, simple) {
    variable_reference expr { vardesc(1) };
    EXPECT_EQ(expr.variable(), vardesc(1));
}

TEST_F(variable_reference_test, clone) {
    variable_reference expr { vardesc(1) };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(variable_reference_test, output) {
    variable_reference expr { vardesc(1) };
    std::cout << expr << std::endl;
}

} // namespace takatori::scalar
