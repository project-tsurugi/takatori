#include <takatori/name/name.h>

#include <type_traits>

#include <gtest/gtest.h>

namespace takatori::name {

class name_test : public ::testing::Test {};

TEST_F(name_test, simple) {
    name n { "a", "b", "c" };
    ASSERT_EQ(n.size(), 3);
    EXPECT_EQ(n[0], "a");
    EXPECT_EQ(n[1], "b");
    EXPECT_EQ(n[2], "c");
    EXPECT_EQ(n.front(), "a");
    EXPECT_EQ(n.back(), "c");
    EXPECT_TRUE(n);
    EXPECT_FALSE(n.empty());
}

TEST_F(name_test, ctor_empty) {
    name n {};
    ASSERT_EQ(n.size(), 0);
    EXPECT_FALSE(n);
    EXPECT_TRUE(n.empty());
}

TEST_F(name_test, ctor_identifier) {
    name n {{
            identifier { "a" },
            identifier { "b" },
            identifier { "c" },
    }};
    ASSERT_EQ(n.size(), 3);
    EXPECT_EQ(n[0], "a");
    EXPECT_EQ(n[1], "b");
    EXPECT_EQ(n[2], "c");
    EXPECT_EQ(n.front(), "a");
    EXPECT_EQ(n.back(), "c");
    EXPECT_TRUE(n);
    EXPECT_FALSE(n.empty());
}

TEST_F(name_test, output) {
    name n { "a", "b", "c" };
    std::cout << n << std::endl;
}

} // namespace takatori::name
