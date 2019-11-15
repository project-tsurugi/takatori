#include "takatori/datetime/date_interval.h"

#include <gtest/gtest.h>

namespace takatori::datetime {

class date_interval_test : public ::testing::Test {};

TEST_F(date_interval_test, zero) {
    date_interval d;
    EXPECT_EQ(d.year(), 0);
    EXPECT_EQ(d.month(), 0);
    EXPECT_EQ(d.day(), 0);
}

TEST_F(date_interval_test, hms) {
    date_interval d { 1, 2, 3 };
    EXPECT_EQ(d.year(), 1);
    EXPECT_EQ(d.month(), 2);
    EXPECT_EQ(d.day(), 3);
}

TEST_F(date_interval_test, negative) {
    date_interval d { -1, -2, -3 };
    EXPECT_EQ(d.year(), -1);
    EXPECT_EQ(d.month(), -2);
    EXPECT_EQ(d.day(), -3);
}

TEST_F(date_interval_test, mix) {
    date_interval d { 1, -2, 3 };
    EXPECT_EQ(d.year(), 1);
    EXPECT_EQ(d.month(), -2);
    EXPECT_EQ(d.day(), 3);
}

TEST_F(date_interval_test, operator_bool) {
    EXPECT_FALSE(date_interval());
    EXPECT_TRUE(date_interval(1));
}

TEST_F(date_interval_test, plus) {
    EXPECT_EQ(+date_interval(1, 2, 3), date_interval(1, 2, 3));
}

TEST_F(date_interval_test, minus) {
    EXPECT_EQ(-date_interval(1, 2, 3), date_interval(-1, -2, -3));
}

TEST_F(date_interval_test, add) {
    EXPECT_EQ(date_interval(1, 2, 3) + date_interval(10, 20, 30), date_interval(11, 22, 33));
}

TEST_F(date_interval_test, subtract) {
    EXPECT_EQ(date_interval(10, 20, 30) - date_interval(1, 2, 3), date_interval(9, 18, 27));
}

TEST_F(date_interval_test, add_assign) {
    date_interval v { 1, 2, 3 };
    v += date_interval { 10, 20, 30 };
    EXPECT_EQ(v, date_interval(11, 22, 33));
}

TEST_F(date_interval_test, subtract_assign) {
    date_interval v { 10, 20, 30 };
    v -= date_interval { 1, 2, 3 };
    EXPECT_EQ(v, date_interval(9, 18, 27));
}

TEST_F(date_interval_test, hash) {
    using t = date_interval;
    std::hash<t> h;
    EXPECT_EQ(h(t(1, 2, 3)), h(t(1, 2, 3)));
    EXPECT_NE(h(t(1, 1, 1)), h(t(1, 1, 2)));
    EXPECT_NE(h(t(1, 1, 1)), h(t(1, 2, 1)));
    EXPECT_NE(h(t(1, 1, 1)), h(t(2, 1, 1)));
}

TEST_F(date_interval_test, output) {
    date_interval d { 1, -2, 3 };
    std::cout << d << std::endl;
}

} // namespace takatori::datetime
