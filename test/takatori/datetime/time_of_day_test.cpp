#include <takatori/datetime/time_of_day.h>

#include <chrono>

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

TEST_F(time_of_day_test, output) {
    time_of_day d { 12, 34, 56, unit { 789'000'000 } };
    std::cout << d << std::endl;
}

TEST_F(time_of_day_test, subtract) {
    namespace c = std::chrono;
    time_of_day d { 12, 34, 56, unit { 789'000'000 } };
    d -= c::hours{9};
    EXPECT_EQ(d.hour(), 3);
    EXPECT_EQ(d.minute(), 34);
    EXPECT_EQ(d.second(), 56);
    EXPECT_EQ(d.subsecond(), c::milliseconds(789));
}

TEST_F(time_of_day_test, subtract_wraps) {
    namespace c = std::chrono;
    time_of_day d { 12, 34, 56, unit { 789'000'000 } };
    d -= c::hours{15};
    EXPECT_EQ(d.hour(), 21);
    EXPECT_EQ(d.minute(), 34);
    EXPECT_EQ(d.second(), 56);
    EXPECT_EQ(d.subsecond(), c::milliseconds(789));
}

TEST_F(time_of_day_test, diff) {
    namespace c = std::chrono;
    time_of_day d0 { 12, 34, 56, unit { 789'000'000 } };
    time_of_day d1 { 23, 34, 56, unit { 789'000'000 } };
    auto diff = d1 - d0;
    EXPECT_EQ(diff, c::hours{11});
}

TEST_F(time_of_day_test, diff_wraps) {
    namespace c = std::chrono;
    time_of_day d0 { 12, 34, 56, unit { 789'000'000 } };
    time_of_day d1 { 23, 34, 56, unit { 789'000'000 } };
    auto diff = d0 - d1;
    EXPECT_EQ(diff, c::hours{13});
}

} // namespace takatori::datetime
