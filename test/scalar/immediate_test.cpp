#include "takatori/scalar/immediate.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

class immediate_test : public ::testing::Test {};

static_assert(immediate::tag == expression_kind::immediate);
static_assert(std::is_same_v<expression_kind_type_t<immediate::tag>, immediate>);

TEST_F(immediate_test, simple) {
    immediate expr {
            value(1),
            typedesc(2),
    };
    EXPECT_EQ(expr.value(), value(1));
    EXPECT_EQ(expr.type(), typedesc(2));
}

TEST_F(immediate_test, clone) {
    immediate expr {
            value(1),
            typedesc(2),
    };
    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(immediate_test, output) {
    immediate expr {
            value(1),
            typedesc(2),
    };
    std::cout << expr << std::endl;
}

} // namespace takatori::scalar
