#include <takatori/util/finalizer.h>

#include <gtest/gtest.h>

namespace takatori::util {

class finalizer_test : public ::testing::Test {};

TEST_F(finalizer_test, simple) {
    int v = 1;
    {
        finalizer f { [&] { ++v; } };
        EXPECT_EQ(v, 1);
    }
    EXPECT_EQ(v, 2);
}

TEST_F(finalizer_test, cancel) {
    int v = 1;
    {
        finalizer f { [&] { ++v; } };
        EXPECT_EQ(v, 1);
        f = []{};
    }
    EXPECT_EQ(v, 1);
}

TEST_F(finalizer_test, move) {
    int v = 1;
    {
        finalizer f;
        {
            finalizer f2 { [&] { ++v; } };
            EXPECT_EQ(v, 1);
            f = std::move(f2);
        }
        EXPECT_EQ(v, 1);
    }
    EXPECT_EQ(v, 2);
}

} // namespace takatori::util
