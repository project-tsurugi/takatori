#include <takatori/value/dispatch.h>

#include <gtest/gtest.h>

namespace takatori::value {

class value_dispatch_test : public ::testing::Test {};

TEST_F(value_dispatch_test, switch) {
    struct cb {
        std::int64_t operator()(unknown const&) { return -1; }
        std::int64_t operator()(int4 const& v) { return v.get() * 4; }
        std::int64_t operator()(int8 const& v) { return v.get() * 8; }
        std::int64_t operator()(data const&) { throw std::domain_error("default"); }
    };
    EXPECT_EQ(dispatch(cb {}, unknown()), -1);
    EXPECT_EQ(dispatch(cb {}, int4(1)), 4);
    EXPECT_EQ(dispatch(cb {}, int8(2)), 16);
    EXPECT_THROW(dispatch(cb {}, boolean()), std::domain_error);
}

} // namespace takatori::value
