#include "util/rvalue_initializer_list.h"

#include <gtest/gtest.h>

namespace util {

class rvalue_initializer_list_test : public ::testing::Test {};

TEST_F(rvalue_initializer_list_test, simple) {
    int a = 1, b = 2, c = 3;
    rvalue_initializer_list<int> v { std::move(a), std::move(b), std::move(c) };
    static_assert(std::is_same_v<typename decltype(v)::value_type, int>);

    EXPECT_EQ(v.size(), 3);

    a = 10;
    b = 20;
    c = 30;

    auto&& iter = v.begin();
    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 10);
    ++iter;

    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 20);
    ++iter;

    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 30);
    ++iter;

    ASSERT_EQ(iter, v.end());
}

TEST_F(rvalue_initializer_list_test, move_assign) {
    auto a = std::make_unique<int>(1);
    auto b = std::make_unique<int>(2);
    auto c = std::make_unique<int>(3);

    rvalue_initializer_list<decltype(a)> v { std::move(a), std::move(b), std::move(c) };

    std::vector<std::unique_ptr<int>> vv { v.begin(), v.end() };

    EXPECT_EQ(vv.size(), 3);
    EXPECT_EQ(*vv[0], 1);
    EXPECT_EQ(*vv[1], 2);
    EXPECT_EQ(*vv[2], 3);
}

} // namespace util
