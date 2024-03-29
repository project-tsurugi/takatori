#include <takatori/type/time_point.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

#include "test_utils.h"

namespace takatori::type {

class time_point_type_test : public ::testing::Test {};

static_assert(time_point::tag == type_kind::time_point);
static_assert(std::is_same_v<type_of_t<time_point::tag>, time_point>);

TEST_F(time_point_type_test, simple) {
    time_point t;
    EXPECT_FALSE(t.with_time_zone());
}

TEST_F(time_point_type_test, time_zone) {
    time_point t { with_time_zone };
    EXPECT_TRUE(t.with_time_zone());
}

TEST_F(time_point_type_test, hash) {
    time_point a;
    time_point b { with_time_zone };

    using namespace std;

    EXPECT_EQ(std::hash<time_point>{}(a), std::hash<time_point>{}(a));
    EXPECT_EQ(std::hash<time_point>{}(b), std::hash<time_point>{}(b));
    EXPECT_NE(std::hash<time_point>{}(a), std::hash<time_point>{}(b));
}

TEST_F(time_point_type_test, clone) {
    time_point t { with_time_zone };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(time_point_type_test, clone_move) {
    time_point t { with_time_zone };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(time_point_type_test, output) {
    time_point t { with_time_zone };
    std::cout << t << std::endl;
}

} // namespace takatori::type
