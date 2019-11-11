#include "takatori/type/time_of_day.h"

#include <gtest/gtest.h>

#include "takatori/util/clonable.h"

#include "test_utils.h"

namespace takatori::type {

class time_of_day_type_test : public ::testing::Test {};


TEST_F(time_of_day_type_test, simple) {
    time_of_day t;
    EXPECT_FALSE(t.time_zone());
}

TEST_F(time_of_day_type_test, time_zone) {
    time_of_day t { tz(1) };
    EXPECT_EQ(t.time_zone(), tz(1));
}

TEST_F(time_of_day_type_test, hash) {
    time_of_day a;
    time_of_day b { tz(1) };

    using namespace std;

    EXPECT_EQ(std::hash<time_of_day>{}(a), std::hash<time_of_day>{}(a));
    EXPECT_EQ(std::hash<time_of_day>{}(b), std::hash<time_of_day>{}(b));
    EXPECT_NE(std::hash<time_of_day>{}(a), std::hash<time_of_day>{}(b));
}

TEST_F(time_of_day_type_test, clone) {
    time_of_day t { tz(1) };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(time_of_day_type_test, clone_move) {
    time_of_day t { tz(1) };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(time_of_day_type_test, output) {
    time_of_day t { tz(1) };
    std::cout << t << std::endl;
}

} // namespace takatori::type
