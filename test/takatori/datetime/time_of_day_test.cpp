#include <takatori/datetime/time_of_day.h>

#include <gtest/gtest.h>

namespace takatori::datetime {

class time_of_day_test : public ::testing::Test {};

using unit = time_of_day::time_unit;

TEST_F(time_of_day_test, epoch) {
    time_of_day d;
    EXPECT_EQ(d.time_since_epoch(), unit(0));
    EXPECT_EQ(d.hour(), 0);
    EXPECT_EQ(d.minute(), 0);
    EXPECT_EQ(d.second(), 0);
    EXPECT_EQ(d.subsecond(), unit(0));
}

TEST_F(time_of_day_test, hms) {
    namespace c = std::chrono;
    time_of_day d { 12, 34, 56, unit { 789'000'000 } };
    EXPECT_EQ(d.time_since_epoch(), c::hours(12) + c::minutes(34) + c::seconds(56) + c::milliseconds(789));
    EXPECT_EQ(d.hour(), 12);
    EXPECT_EQ(d.minute(), 34);
    EXPECT_EQ(d.second(), 56);
    EXPECT_EQ(d.subsecond(), c::milliseconds(789));
}

TEST_F(time_of_day_test, operator_plus_in_range) {
    time_of_day a { 1, 0, 0 };
    auto r = a + std::chrono::hours { 2 };
    EXPECT_EQ(r.time_since_epoch(), std::chrono::hours { 3 });
}

TEST_F(time_of_day_test, operator_plus_wrap_positive) {
    time_of_day a { 12, 0, 0 };
    auto r = a + std::chrono::hours { 13 };
    EXPECT_EQ(r.time_since_epoch(), std::chrono::hours { 1 });
}

TEST_F(time_of_day_test, operator_plus_wrap_negative) {
    time_of_day a { 12, 0, 0 };
    auto r = a + std::chrono::hours { -13 };
    EXPECT_EQ(r.time_since_epoch(), std::chrono::hours { 23 });
}

TEST_F(time_of_day_test, operator_minus_in_range) {
    time_of_day a { 12, 0, 0 };
    time_of_day b { 3, 0, 0 };
    auto r = a - b;
    EXPECT_EQ(r, std::chrono::hours { 9 });
}

TEST_F(time_of_day_test, operator_minus_wrap_negative) {
    time_of_day a { 12, 0, 0 };
    time_of_day b { 13, 0, 0 };
    auto r = a - b;
    EXPECT_EQ(r, std::chrono::hours { -1 });
}

TEST_F(time_of_day_test, output) {
    time_of_day d { 12, 34, 56, unit { 789'000'000 } };
    std::cout << d << std::endl;
}

} // namespace takatori::datetime
