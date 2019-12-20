#include <takatori/datetime/time_interval.h>

#include <gtest/gtest.h>

namespace takatori::datetime {

class time_interval_test : public ::testing::Test {};

using unit = time_interval::time_unit;

TEST_F(time_interval_test, zero) {
    time_interval d;
    EXPECT_EQ(d.offset(), unit(0));
    EXPECT_EQ(d.hour(), 0);
    EXPECT_EQ(d.minute(), 0);
    EXPECT_EQ(d.second(), 0);
    EXPECT_EQ(d.subsecond(), unit(0));
}

TEST_F(time_interval_test, chrono) {
    using namespace std::chrono_literals;
    time_interval d { 36h + 34min + 56s + 789ms };
    EXPECT_EQ(d.offset(), 36h + 34min + 56s + 789ms);
    EXPECT_EQ(d.day(), 1);
    EXPECT_EQ(d.hour(), 12);
    EXPECT_EQ(d.minute(), 34);
    EXPECT_EQ(d.second(), 56);
    EXPECT_EQ(d.subsecond(), 789ms);
}

TEST_F(time_interval_test, negative) {
    using namespace std::chrono_literals;
    time_interval d { -36h - 34min - 56s - 789ms };
    EXPECT_EQ(d.day(), -1);
    EXPECT_EQ(d.hour(), -12);
    EXPECT_EQ(d.minute(), -34);
    EXPECT_EQ(d.second(), -56);
    EXPECT_EQ(d.subsecond(), -789ms);
}

TEST_F(time_interval_test, hms) {
    using namespace std::chrono_literals;
    time_interval d { 36, 34, 56, unit { 789'000'000 } };
    EXPECT_EQ(d.offset(), 36h + 34min + 56s + 789ms);
    EXPECT_EQ(d.day(), 1);
    EXPECT_EQ(d.hour(), 12);
    EXPECT_EQ(d.minute(), 34);
    EXPECT_EQ(d.second(), 56);
    EXPECT_EQ(d.subsecond(), 789ms);
}

TEST_F(time_interval_test, operator_bool) {
    using namespace std::chrono_literals;
    EXPECT_FALSE(time_interval());
    EXPECT_TRUE(time_interval(1s));
}

TEST_F(time_interval_test, plus) {
    using namespace std::chrono_literals;
    EXPECT_EQ(+time_interval(1s), time_interval(1s));
}

TEST_F(time_interval_test, minus) {
    using namespace std::chrono_literals;
    EXPECT_EQ(-time_interval(1s), time_interval(-1s));
}

TEST_F(time_interval_test, add) {
    using namespace std::chrono_literals;
    EXPECT_EQ(time_interval(1s) + time_interval(2s), time_interval(3s));
}

TEST_F(time_interval_test, subtract) {
    using namespace std::chrono_literals;
    EXPECT_EQ(time_interval(1s) - time_interval(3s), time_interval(-2s));
}

TEST_F(time_interval_test, add_assign) {
    using namespace std::chrono_literals;
    time_interval v { 1s };
    v += time_interval { 2s };
    EXPECT_EQ(v, time_interval(3s));
}

TEST_F(time_interval_test, subtract_assign) {
    using namespace std::chrono_literals;
    time_interval v { 1s };
    v -= time_interval { 3s };
    EXPECT_EQ(v, time_interval(-2s));
}

TEST_F(time_interval_test, hash) {
    using namespace std::chrono_literals;
    using t = time_interval;
    std::hash<t> h;
    EXPECT_EQ(h(t(1s)), h(t(1s)));
    EXPECT_EQ(h(t(-1s)), h(t(-1s)));
    EXPECT_NE(h(t(+1s)), h(t(-1s)));
}

TEST_F(time_interval_test, output) {
    using namespace std::chrono_literals;
    time_interval d { 26h + 3min + 4s + 5ms + 67us + 890ns };
    std::cout << d << std::endl;
}

} // namespace takatori::datetime
