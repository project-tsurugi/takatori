#include "takatori/datetime/datetime_interval.h"

#include <gtest/gtest.h>

#include "takatori/datetime/interval.h"

namespace takatori::datetime {

class datetime_interval_test : public ::testing::Test {};

TEST_F(datetime_interval_test, zero) {
    datetime_interval v;
    EXPECT_FALSE(v.date());
    EXPECT_FALSE(v.time());
}

TEST_F(datetime_interval_test, date_time) {
    datetime_interval v { {1, 2, 3}, {4, 5, 6} };
    EXPECT_EQ(v.date(), date_interval(1, 2, 3));
    EXPECT_EQ(v.time(), time_interval(4, 5, 6));
}

TEST_F(datetime_interval_test, plus) {
    EXPECT_EQ(+datetime_interval({1, 2, 3}, {4, 5, 6}), datetime_interval({1, 2, 3}, {4, 5, 6}));
}

TEST_F(datetime_interval_test, minus) {
    EXPECT_EQ(-datetime_interval({1, 2, 3}, {4, 5, 6}), datetime_interval({-1, -2, -3}, {-4, -5, -6}));
}

TEST_F(datetime_interval_test, add) {
    EXPECT_EQ(
            datetime_interval({1, 2, 3}, {4, 5, 6}) + datetime_interval({10, 20, 30}, {40, 50, 60}),
            datetime_interval({11, 22, 33}, {44, 55, 66}));
}

TEST_F(datetime_interval_test, add_hetero) {
    EXPECT_EQ(
            date_interval(1, 2, 3) + time_interval(4, 5, 6),
            datetime_interval({1, 2, 3}, {4, 5, 6}));
}

TEST_F(datetime_interval_test, subtract) {
    EXPECT_EQ(
            datetime_interval({10, 20, 30}, {40, 50, 60}) - datetime_interval({1, 2, 3}, {4, 5, 6}),
            datetime_interval({9, 18, 27}, {36, 45, 54}));
}

TEST_F(datetime_interval_test, add_assign) {
    datetime_interval v { {1, 2, 3}, {4, 5, 6} };
    v += datetime_interval({10, 20, 30}, {40, 50, 60});
    EXPECT_EQ(v, datetime_interval({11, 22, 33}, {44, 55, 66}));
}

TEST_F(datetime_interval_test, subtract_assign) {
    datetime_interval v { {10, 20, 30}, {40, 50, 60} };
    v -= datetime_interval({1, 2, 3}, {4, 5, 6});
    EXPECT_EQ(v, datetime_interval({9, 18, 27}, {36, 45, 54}));
}

TEST_F(datetime_interval_test, hash) {
    using t = datetime_interval;
    std::hash<t> h;
    EXPECT_EQ(h(t({ 1, 2, 3 }, { 4, 5, 6 })), h(t({ 1, 2, 3 }, { 4, 5, 6 })));
    EXPECT_EQ(h(t({ 1, 1, 1 }, { 1, 1, 1 })), h(t({ 1, 1, 1 }, { 1, 1, 1 })));
    EXPECT_NE(h(t({ 1, 1, 1 }, { 1, 1, 1 })), h(t({ 1, 1, 1 }, { 1, 1, 2 })));
    EXPECT_NE(h(t({ 1, 1, 1 }, { 1, 1, 1 })), h(t({ 1, 1, 1 }, { 1, 2, 1 })));
    EXPECT_NE(h(t({ 1, 1, 1 }, { 1, 1, 1 })), h(t({ 1, 1, 1 }, { 2, 1, 1 })));
    EXPECT_NE(h(t({ 1, 1, 1 }, { 1, 1, 1 })), h(t({ 1, 1, 2 }, { 1, 1, 1 })));
    EXPECT_NE(h(t({ 1, 1, 1 }, { 1, 1, 1 })), h(t({ 1, 2, 1 }, { 1, 1, 1 })));
    EXPECT_NE(h(t({ 1, 1, 1 }, { 1, 1, 1 })), h(t({ 2, 1, 1 }, { 1, 1, 1 })));
}

TEST_F(datetime_interval_test, output) {
    datetime_interval v { {1, 2, 3}, {4, 5, 6} };
    std::cout << v << std::endl;
}

} // namespace takatori::datetime
