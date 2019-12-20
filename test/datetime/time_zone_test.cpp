#include <takatori/datetime/time_zone.h>

#include <gtest/gtest.h>

namespace takatori::datetime {

class time_zone_test : public ::testing::Test {};

using namespace std::chrono_literals;

TEST_F(time_zone_test, utc) {
    time_zone tz;
    EXPECT_EQ(tz.name(), "UTC");
    EXPECT_EQ(tz, time_zone::UTC);
}

TEST_F(time_zone_test, ctor_symbol_abbreviation) {
    time_zone tz { "JST" };
    EXPECT_EQ(tz.name(), "JST");
    EXPECT_EQ(tz.offset(), +9h);
}

TEST_F(time_zone_test, ctor_symbol_area_location) {
    time_zone tz { "Asia/Tokyo" };
    EXPECT_EQ(tz.name(), "Asia/Tokyo");
    EXPECT_EQ(tz.offset(), +9h);
}

TEST_F(time_zone_test, ctor_symbol_offset) {
    time_zone tz { "GMT+9" };
    EXPECT_EQ(tz.name(), "GMT+9");
    EXPECT_EQ(tz.offset(), +9h);
}

TEST_F(time_zone_test, ctor_symbol_invalid) {
    EXPECT_THROW((time_zone { "?" }), std::invalid_argument);
}

TEST_F(time_zone_test, ctor_offset) {
    time_zone tz { +9h };
    EXPECT_EQ(tz.offset(), +9h);
}

TEST_F(time_zone_test, ctor_offset_out_of_range) {
    EXPECT_THROW((time_zone { +24h }), std::out_of_range);
    EXPECT_THROW((time_zone { -24h }), std::out_of_range);
}

TEST_F(time_zone_test, find_time_zone) {
    auto otz = find_time_zone("PST");
    ASSERT_TRUE(otz);

    auto&& tz = otz.value();
    EXPECT_EQ(tz.name(), "PST");
    EXPECT_EQ(tz.offset(), -8h);

    EXPECT_FALSE(find_time_zone("?"));
}

// some environment does not resolves abbr into area/location.
TEST_F(time_zone_test, DISABLED_id) {
    time_zone jst_abbr { "JST" };
    time_zone jst_area { "Asia/Tokyo" };

    time_zone pst_abbr { "PST" };
    time_zone pst_area { "America/Los_Angeles" };

    EXPECT_EQ(jst_abbr, jst_area);
    EXPECT_EQ(jst_abbr.id(), jst_area.id());

    EXPECT_EQ(pst_abbr, pst_area);
    EXPECT_EQ(pst_abbr.id(), pst_area.id());

    EXPECT_NE(jst_area, pst_area);
    EXPECT_NE(jst_area.id(), pst_area.id());
}

TEST_F(time_zone_test, id_offset) {
    time_zone a { "GMT+9" };
    time_zone b { +9h };

    EXPECT_EQ(a, b);
    EXPECT_EQ(a.id(), b.id()) << a.id();
}

TEST_F(time_zone_test, output) {
    time_zone tz { "JST" };
    std::cout << tz << std::endl;
}

} // namespace takatori::datetime
