#include <takatori/util/fail.h>

#include <gtest/gtest.h>

namespace takatori::util {

class fail_test : public ::testing::Test {};

TEST_F(fail_test, simple) {
    EXPECT_DEATH(fail(), "");
}

TEST_F(fail_test, message) {
    EXPECT_DEATH(fail("!!!"), "!!!");
}

} // namespace takatori::util
