#include "model/scalar/expression_dispatch.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "util/clonable.h"

namespace model::scalar {

class expression_dispatch_test : public ::testing::Test {};

TEST_F(expression_dispatch_test, simple) {
    struct cb {
        int operator()(expression const&) { std::abort(); }
        int operator()(immediate const& expr) {
            return resolve(expr.value());
        }
    };

    auto v = dispatch(cb {}, constant(100));
    EXPECT_EQ(v, 100);
}

TEST_F(expression_dispatch_test, switch) {
    struct cb {
        int operator()(expression const&) { std::abort(); }
        int operator()(immediate const&) {
            return 1;
        }
        int operator()(variable_reference const&) {
            return 2;
        }
    };

    EXPECT_EQ(dispatch(cb {}, constant(0)), 1);
    EXPECT_EQ(dispatch(cb {}, vref(0)), 2);
}

TEST_F(expression_dispatch_test, void) {
    struct cb {
        void operator()(expression const&) { std::abort(); }
        void operator()(immediate const&) {
            r = 1;
        }
        void operator()(variable_reference const&) {
            r = 2;
        }
        int r;
    };

    cb c;

    dispatch(c, constant(0));
    EXPECT_EQ(c.r, 1);

    dispatch(c, vref(0));
    EXPECT_EQ(c.r, 2);
}

} // namespace model::scalar
