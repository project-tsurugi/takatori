#include "takatori/datetime/time_point.h"

#include <gtest/gtest.h>

namespace takatori::datetime {

class time_point_test : public ::testing::Test {};

using unit = time_point::time_unit;

TEST_F(time_point_test, epoch) {
    time_point tp;
    EXPECT_EQ(tp.time_since_epoch(), unit(0));
    EXPECT_EQ(tp.date(), date());
    EXPECT_EQ(tp.time(), time_of_day());
}

TEST_F(time_point_test, unix_time) {
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> s { std::chrono::milliseconds::zero() };
    time_point tp { s };
    EXPECT_EQ(tp.date(), date(1970, 1, 1));
    EXPECT_EQ(tp.time(), time_of_day());
}

TEST_F(time_point_test, date_time) {
    time_point tp {
        date(1901, 2, 3),
        time_of_day(4, 5, 6, std::chrono::milliseconds(789)) };
    EXPECT_EQ(tp.date(), date(1901, 2, 3));
    EXPECT_EQ(tp.time(), time_of_day(4, 5, 6, std::chrono::milliseconds(789)));
}

TEST_F(time_point_test, output) {
    std::cout << "now: " << time_point::now() << std::endl;
}

} // namespace takatori::datetime
