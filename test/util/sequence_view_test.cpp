#include <takatori/util/sequence_view.h>

#include <array>
#include <vector>

#include <gtest/gtest.h>

namespace takatori::util {

class sequence_view_test : public ::testing::Test {};

TEST_F(sequence_view_test, simple) {
    std::array<int, 3> a { 1, 2, 3 };
    sequence_view v { a };
    static_assert(std::is_same_v<decltype(v)::value_type, int>);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(sequence_view_test, empty) {
    sequence_view<int> v;
    static_assert(std::is_same_v<decltype(v)::value_type, int>);

    ASSERT_EQ(v.size(), 0);
    EXPECT_EQ(v.begin(), v.end());
}

TEST_F(sequence_view_test, pointer_size) {
    std::array<int, 5> a { 1, 2, 3, 4, 5 };
    sequence_view v { &a[1], 3 };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);
}

TEST_F(sequence_view_test, pointer_range) {
    std::array<int, 5> a { 1, 2, 3, 4, 5 };
    sequence_view v { &a[1], &a[4] };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);
}

TEST_F(sequence_view_test, container) {
    std::vector a { 1, 2, 3 };
    sequence_view v { a };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(sequence_view_test, at) {
    std::array<int, 3> a { 1, 2, 3 };
    sequence_view v { a };

    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);
    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST_F(sequence_view_test, front_back) {
    std::array<int, 3> a { 1, 2, 3 };
    sequence_view v { a };

    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
}

TEST_F(sequence_view_test, data) {
    std::array<int, 3> a { 1, 2, 3 };
    sequence_view v { a };

    EXPECT_EQ(v.data(), &a[0]);
}

TEST_F(sequence_view_test, begin_end) {
    std::array<int, 3> a { 1, 2, 3 };
    sequence_view v { a };

    auto iter = v.begin();
    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 1);

    ++iter;
    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 2);

    ++iter;
    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 3);

    ++iter;
    ASSERT_EQ(iter, v.end());
}

TEST_F(sequence_view_test, rbegin_rend) {
    std::array<int, 3> a { 1, 2, 3 };
    sequence_view v { a };

    auto iter = v.rbegin();
    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 3);

    ++iter;
    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 2);

    ++iter;
    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 1);

    ++iter;
    ASSERT_EQ(iter, v.rend());
}

} // namespace takatori::util
