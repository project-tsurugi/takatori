#include <takatori/scalar/immediate.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/type/int.h>
#include <takatori/value/int.h>

#include <takatori/util/clonable.h>

namespace takatori::scalar {

class immediate_test : public ::testing::Test {};

static_assert(immediate::tag == expression_kind::immediate);
static_assert(std::is_same_v<type_of_t<immediate::tag>, immediate>);

TEST_F(immediate_test, simple) {
    immediate expr {
            value::int4(1),
            type::int4(),
    };
    EXPECT_EQ(expr.value(), value::int4(1));
    EXPECT_EQ(expr.type(), type::int4());
}

TEST_F(immediate_test, clone) {
    immediate expr {
            value::int4(1),
            type::int4(),
    };
    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(immediate_test, output) {
    immediate expr {
            value::int4(1),
            type::int4(),
    };
    std::cout << expr << std::endl;
}

} // namespace takatori::scalar
