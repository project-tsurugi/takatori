#include <takatori/relation/intermediate/dispatch.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include <takatori/relation/step/distinct.h>

namespace takatori::relation::intermediate {

class relation_intermediate_dispatch_test : public ::testing::Test {};

TEST_F(relation_intermediate_dispatch_test, simple) {
    struct cb {
        int operator()(distinct const& op) { return value_of<int>(op.keys()[0]); }
        int operator()(expression const&) { return 0; }
    };
    distinct e { vardesc(1) };
    auto r = dispatch(cb {}, e);
    EXPECT_EQ(r, 1);
}

TEST_F(relation_intermediate_dispatch_test, common) {
    struct cb {
        int operator()(filter const& op) { return value_of<value::int4>(op.condition()); }
        int operator()(expression const&) { return 0; }
    };
    filter e { constant(100) };
    auto r = dispatch(cb {}, e);
    EXPECT_EQ(r, 100);
}

TEST_F(relation_intermediate_dispatch_test, switch) {
    struct cb {
        int operator()(distinct const&) { return 1; }
        int operator()(intersection const&) { return 2; }
        int operator()(difference const&) { return 3; }
        int operator()(expression const&) { return -1; }
    };
    EXPECT_EQ(dispatch(cb {}, distinct { vardesc(1) }), 1);
    EXPECT_EQ(dispatch(cb {}, intersection { { vardesc(1), vardesc(2) } }), 2);
    EXPECT_EQ(dispatch(cb {}, difference { { vardesc(1), vardesc(2) } }), 3);
    EXPECT_EQ(dispatch(cb {}, limit { 10 }), -1);
}

TEST_F(relation_intermediate_dispatch_test, unsupported) {
    struct cb {
        int operator()(expression const&) { return 0; }
    };
    step::distinct e;
    EXPECT_THROW(dispatch(cb {}, e), std::invalid_argument);
}

} // namespace takatori::relation::intermediate
