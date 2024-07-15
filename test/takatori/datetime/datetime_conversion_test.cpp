#include <takatori/datetime/conversion.h>

#include <gtest/gtest.h>

#include <memory>

namespace takatori::datetime {

class datetime_conversion_test : public ::testing::Test {};

TEST_F(datetime_conversion_test, parse_date) {
    auto r = parse_date("2024-09-16");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_EQ(info.year, 2024);
    EXPECT_EQ(info.month, 9);
    EXPECT_EQ(info.day, 16);
}

TEST_F(datetime_conversion_test, parse_date_invalid_field) {
    EXPECT_TRUE(parse_date("1-1-1"));
    EXPECT_FALSE(parse_date("YYYY-MM-DD"));

    EXPECT_FALSE(parse_date("0-1-1"));
    EXPECT_FALSE(parse_date("1-0-1"));
    EXPECT_FALSE(parse_date("1-1-0"));

    EXPECT_FALSE(parse_date("1-13-1"));
    EXPECT_FALSE(parse_date("1-1-32"));
}

TEST_F(datetime_conversion_test, parse_date_invalid_form) {
    EXPECT_FALSE(parse_date("00:00:00"));
    EXPECT_FALSE(parse_date("2024-09-16 00:00:00"));
}

TEST_F(datetime_conversion_test, parse_time) {
    auto r = parse_time("12:34:56.789");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_EQ(info.hour, 12);
    EXPECT_EQ(info.minute, 34);
    EXPECT_EQ(info.second, 56);
    EXPECT_EQ(info.subsecond, time_info::subsecond_type(789'000'000));
}

TEST_F(datetime_conversion_test, parse_time_invalid_field) {
    EXPECT_TRUE(parse_time("0:0:0.0"));
    EXPECT_FALSE(parse_time("HH:MM:SS.NNNNNNNNNN"));

    EXPECT_FALSE(parse_time("0:0:0."));
    EXPECT_FALSE(parse_time("24:0:0.0"));
    EXPECT_FALSE(parse_time("0:60:0.0"));
    EXPECT_FALSE(parse_time("0:0:60.0"));
    EXPECT_FALSE(parse_time("0:0:0.0000000001"));
}

TEST_F(datetime_conversion_test, parse_time_invalid_form) {
    EXPECT_FALSE(parse_time("1970-1-1"));
    EXPECT_FALSE(parse_time("0:0:0Z"));
    EXPECT_FALSE(parse_time("+09"));
}

TEST_F(datetime_conversion_test, parse_datetime) {
    auto r = parse_datetime("1970-01-02 12:34:56.789");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_EQ(info.date.year, 1970);
    EXPECT_EQ(info.date.month, 1);
    EXPECT_EQ(info.date.day, 2);

    EXPECT_EQ(info.time.hour, 12);
    EXPECT_EQ(info.time.minute, 34);
    EXPECT_EQ(info.time.second, 56);
    EXPECT_EQ(info.time.subsecond, time_info::subsecond_type(789'000'000));

    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_conversion_test, parse_datetime_with_offset) {
    auto r = parse_datetime("1970-01-02 12:34:56.789Z");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_EQ(info.date.year, 1970);
    EXPECT_EQ(info.date.month, 1);
    EXPECT_EQ(info.date.day, 2);

    EXPECT_EQ(info.time.hour, 12);
    EXPECT_EQ(info.time.minute, 34);
    EXPECT_EQ(info.time.second, 56);
    EXPECT_EQ(info.time.subsecond, time_info::subsecond_type(789'000'000));

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 0);
    EXPECT_EQ(info.offset->minute, 0);
}

TEST_F(datetime_conversion_test, parse_datetime_with_offset_numeric) {
    auto r = parse_datetime("1970-01-02 12:34:56.789+09");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_EQ(info.date.year, 1970);
    EXPECT_EQ(info.date.month, 1);
    EXPECT_EQ(info.date.day, 2);

    EXPECT_EQ(info.time.hour, 12);
    EXPECT_EQ(info.time.minute, 34);
    EXPECT_EQ(info.time.second, 56);
    EXPECT_EQ(info.time.subsecond, time_info::subsecond_type(789'000'000));

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 9);
    EXPECT_EQ(info.offset->minute, 0);
}

TEST_F(datetime_conversion_test, parse_datetime_without_time) {
    auto r = parse_datetime("1970-01-02");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_EQ(info.date.year, 1970);
    EXPECT_EQ(info.date.month, 1);
    EXPECT_EQ(info.date.day, 2);

    EXPECT_EQ(info.time.hour, 0);
    EXPECT_EQ(info.time.minute, 0);
    EXPECT_EQ(info.time.second, 0);
    EXPECT_EQ(info.time.subsecond, time_info::subsecond_type(0));

    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_conversion_test, parse_datetime_invalid_field) {
    EXPECT_TRUE(parse_datetime("1-1-1 0:0:0.0"));

    EXPECT_FALSE(parse_datetime("0-1-1 0:0:0.0"));
    EXPECT_FALSE(parse_datetime("1-0-1 0:0:0.0"));
    EXPECT_FALSE(parse_datetime("1-1-0 0:0:0.0"));

    EXPECT_FALSE(parse_datetime("1-13-1 0:0:0.0"));
    EXPECT_FALSE(parse_datetime("1-1-32 0:0:0.0"));
    EXPECT_FALSE(parse_datetime("1-1-1 24:0:0.0"));
    EXPECT_FALSE(parse_datetime("1-1-1 0:60:0.0"));
    EXPECT_FALSE(parse_datetime("1-1-1 0:0:60.0000000001"));

    EXPECT_TRUE(parse_datetime("1-1-1 0:0:0.0+0:0"));
    EXPECT_FALSE(parse_datetime("1-1-1 0:0:0.0+24:0"));
    EXPECT_FALSE(parse_datetime("1-1-1 0:0:0.0+0:60"));
    EXPECT_FALSE(parse_datetime("1-1-1 0:0:0.0-24:0"));
    EXPECT_FALSE(parse_datetime("1-1-1 0:0:0.0-0:60"));
}

TEST_F(datetime_conversion_test, parse_datetime_invalid_form) {
    EXPECT_FALSE(parse_datetime("0:0:0.0"));
    EXPECT_FALSE(parse_datetime("Z"));
}

TEST_F(datetime_conversion_test, parse_zone_offset) {
    auto r = parse_zone_offset("Z");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_TRUE(info.plus);
    EXPECT_EQ(info.hour, 0);
    EXPECT_EQ(info.minute, 0);
}

TEST_F(datetime_conversion_test, parse_zone_offset_numeric) {
    auto r = parse_zone_offset("+9:00");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_TRUE(info.plus);
    EXPECT_EQ(info.hour, 9);
    EXPECT_EQ(info.minute, 0);
}

TEST_F(datetime_conversion_test, parse_zone_offset_hour) {
    auto r = parse_zone_offset("-12:34");
    ASSERT_TRUE(r.has_value()) << r.error();
    auto&& info = r.value();

    EXPECT_FALSE(info.plus);
    EXPECT_EQ(info.hour, 12);
    EXPECT_EQ(info.minute, 34);
}

TEST_F(datetime_conversion_test, parse_zone_offset_invalid_field) {
    EXPECT_TRUE(parse_zone_offset("+0:0"));
    EXPECT_FALSE(parse_zone_offset("+24:0"));
    EXPECT_FALSE(parse_zone_offset("+0:60"));

    EXPECT_TRUE(parse_zone_offset("-0:0"));
    EXPECT_FALSE(parse_zone_offset("-24:0"));
    EXPECT_FALSE(parse_zone_offset("-0:60"));

    EXPECT_TRUE(parse_zone_offset("+0"));
    EXPECT_FALSE(parse_zone_offset("+24"));
}

TEST_F(datetime_conversion_test, parse_zone_offset_invalid_form) {
    EXPECT_FALSE(parse_zone_offset("1970-01-01 12:34:56Z"));
    EXPECT_FALSE(parse_zone_offset("1970-01-01"));
    EXPECT_FALSE(parse_zone_offset("12:34:56"));
}

} // namespace takatori::datetime
