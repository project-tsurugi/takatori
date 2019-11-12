#include "takatori/type/dispatch.h"

#include <gtest/gtest.h>

namespace takatori::type {

class type_dispatch_test : public ::testing::Test {};

TEST_F(type_dispatch_test, switch) {
    struct cb {
        int operator()(int1 const&) { return 1; }
        int operator()(int2 const&) { return 2; }
        int operator()(int4 const&) { return 4; }
        int operator()(int8 const&) { return 8; }
        int operator()(data const&) { throw std::domain_error("default"); }
    };
    EXPECT_EQ(dispatch(cb {}, int1()), 1);
    EXPECT_EQ(dispatch(cb {}, int2()), 2);
    EXPECT_EQ(dispatch(cb {}, int4()), 4);
    EXPECT_EQ(dispatch(cb {}, int8()), 8);
    EXPECT_THROW(dispatch(cb {}, unknown()), std::domain_error);
}

} // namespace takatori::type
