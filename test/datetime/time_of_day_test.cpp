#include "takatori/datetime/time_of_day.h"

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

} // namespace takatori::datetime
