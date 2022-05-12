#include <takatori/datetime/time_point.h>

#include <gtest/gtest.h>

namespace takatori::datetime {

class time_point_test : public ::testing::Test {};

using seconds = time_point::offset_type;
using nanos = time_point::subsecond_unit;
using diffs = time_point::difference_type;

TEST_F(time_point_test, epoch) {
    time_point tp;
    EXPECT_EQ(tp.seconds_since_epoch(), seconds(0));
    EXPECT_EQ(tp.subsecond(), nanos(0));
    EXPECT_EQ(tp.date(), date());
    EXPECT_EQ(tp.time(), time_of_day());
}

TEST_F(time_point_test, moveup) {
    time_point tp { seconds(0), diffs(1'234'567'890) };
    EXPECT_EQ(tp.seconds_since_epoch(), seconds(1));
    EXPECT_EQ(tp.subsecond(), nanos(234'567'890));
    EXPECT_EQ(tp.date(), date());
    EXPECT_EQ(tp.time(), time_of_day(nanos(1'234'567'890)));
}

TEST_F(time_point_test, movedown) {
    time_point tp { seconds(0), diffs(-1) };
    EXPECT_EQ(tp.seconds_since_epoch(), seconds(-1));
    EXPECT_EQ(tp.subsecond(), nanos(999'999'999));
    EXPECT_EQ(tp.date(), date(1969, 12, 31));
    EXPECT_EQ(tp.time(), time_of_day(23, 59, 59, nanos(999'999'999)));
}

TEST_F(time_point_test, unix_time) {
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> s { std::chrono::milliseconds::zero() };
    time_point tp { s };
    EXPECT_EQ(tp.date(), date());
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
