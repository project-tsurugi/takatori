#include <takatori/util/assertion.h>

#include <gtest/gtest.h>

namespace takatori::util {

class assertion_test : public ::testing::Test {};

TEST_F(assertion_test, simple) {
    EXPECT_DEBUG_DEATH(BOOST_ASSERT(false), "Assertion");
}

TEST_F(assertion_test, msg) {
    EXPECT_DEBUG_DEATH(BOOST_ASSERT_MSG(false, "!!!"), "!!!");
}

} // namespace takatori::util
