#include "takatori/relation/step/dispatch.h"

#include <gtest/gtest.h>

#include "../test_utils.h"

#include "takatori/relation/intermediate/distinct.h"

namespace takatori::relation::step {

class relation_step_dispatch_test : public ::testing::Test {};

TEST_F(relation_step_dispatch_test, simple) {
    struct cb {
        int operator()(limit const& op) { return static_cast<int>(op.count()); }
        int operator()(expression const&) { return 0; }
    };
    limit e { 100 };
    auto r = dispatch(cb {}, e);
    EXPECT_EQ(r, 100);
}

TEST_F(relation_step_dispatch_test, common) {
    struct cb {
        int operator()(filter const& op) { return value_of<value::int4>(op.condition()); }
        int operator()(expression const&) { return 0; }
    };
    filter e { constant(100) };
    auto r = dispatch(cb {}, e);
    EXPECT_EQ(r, 100);
}

TEST_F(relation_step_dispatch_test, switch) {
    struct cb {
        int operator()(distinct const&) { return 1; }
        int operator()(intersection const&) { return 2; }
        int operator()(difference const&) { return 3; }
        int operator()(expression const&) { return -1; }
    };
    EXPECT_EQ(dispatch(cb {}, distinct {}), 1);
    EXPECT_EQ(dispatch(cb {}, intersection {}), 2);
    EXPECT_EQ(dispatch(cb {}, difference {}), 3);
    EXPECT_EQ(dispatch(cb {}, limit { 10 }), -1);
}

TEST_F(relation_step_dispatch_test, unsupported) {
    struct cb {
        int operator()(expression const&) { return 0; }
    };
    intermediate::distinct e { vardesc(1) };
    EXPECT_THROW(dispatch(cb {}, e), std::invalid_argument);
}

} // namespace takatori::relation::step
