#include <takatori/datetime/parser/parser.h>

#include <gtest/gtest.h>

namespace takatori::datetime::parser {

using subsecond_type = time_info::subsecond_type;

class datetime_parser_test : public ::testing::Test {
protected:
    std::string_view error(parser::result_type const& result) {
        if (result.has_error()) {
            return result.error().message;
        }
        return {};
    }
};

TEST_F(datetime_parser_test, date) {
    auto r = parse("2024-09-16");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    ASSERT_TRUE(info.date);
    EXPECT_EQ(info.date->year, 2024);
    EXPECT_EQ(info.date->month, 9);
    EXPECT_EQ(info.date->day, 16);

    EXPECT_FALSE(info.time);
    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_parser_test, time) {
    auto r = parse("12:34:56");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 12);
    EXPECT_EQ(info.time->minute, 34);
    EXPECT_EQ(info.time->second, 56);
    EXPECT_EQ(info.time->subsecond, subsecond_type(0));

    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_parser_test, time_millis) {
    auto r = parse("12:34:56.789");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 12);
    EXPECT_EQ(info.time->minute, 34);
    EXPECT_EQ(info.time->second, 56);
    EXPECT_EQ(info.time->subsecond, subsecond_type(789'000'000));

    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_parser_test, time_micros) {
    auto r = parse("12:34:56.000789");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 12);
    EXPECT_EQ(info.time->minute, 34);
    EXPECT_EQ(info.time->second, 56);
    EXPECT_EQ(info.time->subsecond, subsecond_type(789'000));

    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_parser_test, time_nanos) {
    auto r = parse("12:34:56.000000789");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 12);
    EXPECT_EQ(info.time->minute, 34);
    EXPECT_EQ(info.time->second, 56);
    EXPECT_EQ(info.time->subsecond, subsecond_type(789));

    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_parser_test, time_offset_plus) {
    auto r = parse("1:2:3+4:5", parser().set_debug());
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 1);
    EXPECT_EQ(info.time->minute, 2);
    EXPECT_EQ(info.time->second, 3);
    EXPECT_EQ(info.time->subsecond, subsecond_type(0));

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 4);
    EXPECT_EQ(info.offset->minute, 5);
}

TEST_F(datetime_parser_test, time_offset_minus) {
    auto r = parse("01:02:03-04:00", parser().set_debug());
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 1);
    EXPECT_EQ(info.time->minute, 2);
    EXPECT_EQ(info.time->second, 3);
    EXPECT_EQ(info.time->subsecond, subsecond_type(0));

    ASSERT_TRUE(info.offset);
    EXPECT_FALSE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 4);
    EXPECT_EQ(info.offset->minute, 00);
}

TEST_F(datetime_parser_test, time_offset_x) {
    auto r = parse("01:02:03Z", parser().set_debug());
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 1);
    EXPECT_EQ(info.time->minute, 2);
    EXPECT_EQ(info.time->second, 3);
    EXPECT_EQ(info.time->subsecond, subsecond_type(0));

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 0);
    EXPECT_EQ(info.offset->minute, 0);
}

TEST_F(datetime_parser_test, datetime) {
    auto r = parse("1970-01-02 03:04:05");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    ASSERT_TRUE(info.date);
    EXPECT_EQ(info.date->year, 1970);
    EXPECT_EQ(info.date->month, 1);
    EXPECT_EQ(info.date->day, 2);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 3);
    EXPECT_EQ(info.time->minute, 4);
    EXPECT_EQ(info.time->second, 5);
    EXPECT_EQ(info.time->subsecond, subsecond_type(0));

    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_parser_test, datetime_t) {
    auto r = parse("1970-01-02T03:04:05");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    ASSERT_TRUE(info.date);
    EXPECT_EQ(info.date->year, 1970);
    EXPECT_EQ(info.date->month, 1);
    EXPECT_EQ(info.date->day, 2);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 3);
    EXPECT_EQ(info.time->minute, 4);
    EXPECT_EQ(info.time->second, 5);
    EXPECT_EQ(info.time->subsecond, subsecond_type(0));

    EXPECT_FALSE(info.offset);
}

TEST_F(datetime_parser_test, datetime_offset) {
    auto r = parse("1970-01-02 03:04:05+06:07");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    ASSERT_TRUE(info.date);
    EXPECT_EQ(info.date->year, 1970);
    EXPECT_EQ(info.date->month, 1);
    EXPECT_EQ(info.date->day, 2);

    ASSERT_TRUE(info.time);
    EXPECT_EQ(info.time->hour, 3);
    EXPECT_EQ(info.time->minute, 4);
    EXPECT_EQ(info.time->second, 5);
    EXPECT_EQ(info.time->subsecond, subsecond_type(0));

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 6);
    EXPECT_EQ(info.offset->minute, 7);
}

TEST_F(datetime_parser_test, offset) {
    auto r = parse("+01:02");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);
    EXPECT_FALSE(info.time);

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 1);
    EXPECT_EQ(info.offset->minute, 2);
}

TEST_F(datetime_parser_test, offset_minus) {
    auto r = parse("-01:02");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);
    EXPECT_FALSE(info.time);

    ASSERT_TRUE(info.offset);
    EXPECT_FALSE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 1);
    EXPECT_EQ(info.offset->minute, 2);
}

TEST_F(datetime_parser_test, offset_z) {
    auto r = parse("Z");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);
    EXPECT_FALSE(info.time);

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 0);
    EXPECT_EQ(info.offset->minute, 0);
}

TEST_F(datetime_parser_test, offset_hour) {
    auto r = parse("+09");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);
    EXPECT_FALSE(info.time);

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 9);
    EXPECT_EQ(info.offset->minute, 0);
}

TEST_F(datetime_parser_test, offset_without_colon) {
    auto r = parse("+0102");
    ASSERT_TRUE(r.has_value()) << error(r);
    auto&& info = r.value();

    EXPECT_FALSE(info.date);
    EXPECT_FALSE(info.time);

    ASSERT_TRUE(info.offset);
    EXPECT_TRUE(info.offset->plus);
    EXPECT_EQ(info.offset->hour, 1);
    EXPECT_EQ(info.offset->minute, 2);
}

} // namespace takatori::datetime::parser
