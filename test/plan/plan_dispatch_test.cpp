#include "takatori/plan/dispatch.h"

#include <gtest/gtest.h>

namespace takatori::plan {

class plan_dispatch_test : public ::testing::Test {};

TEST_F(plan_dispatch_test, simple) {
    struct cb {
        int operator()(forward const& op) { return static_cast<int>(op.limit().value()); }
        int operator()(step const&) { return 0; }
    };
    forward e { 100 };
    auto r = dispatch(cb {}, e);
    EXPECT_EQ(r, 100);
}

TEST_F(plan_dispatch_test, switch) {
    struct cb {
        int operator()(process const&) { return 1; }
        int operator()(forward const&) { return 2; }
        int operator()(broadcast const&) { return 3; }
        int operator()(step const&) { return -1; }
    };
    EXPECT_EQ(dispatch(cb {}, process {}), 1);
    EXPECT_EQ(dispatch(cb {}, forward {}), 2);
    EXPECT_EQ(dispatch(cb {}, broadcast{}), 3);
    EXPECT_EQ(dispatch(cb {}, discard {}), -1);
}

} // namespace takatori::plan
