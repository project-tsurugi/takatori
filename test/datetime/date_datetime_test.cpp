#include "takatori/datetime/date.h"

#include <memory>

#include <unicode/calendar.h>

#include <gtest/gtest.h>

namespace takatori::datetime {

class date_datetime_test : public ::testing::Test {};

TEST_F(date_datetime_test, epoch) {
    date d;
    EXPECT_EQ(d.elapsed(), 0);
    EXPECT_EQ(d.year(), 1900);
    EXPECT_EQ(d.month(), 1);
    EXPECT_EQ(d.day(), 1);
}

TEST_F(date_datetime_test, conversion) {
    UErrorCode status = U_ZERO_ERROR;
    std::unique_ptr<icu::Calendar> calendar {icu::Calendar::createInstance(
            *icu::TimeZone::getGMT(),
            icu::Locale::getEnglish(),
            status)
    };
    // set calendar to epoch
    calendar->set(1900, 0, 1, 0, 0, 0);

    for (std::uint32_t i = 0, n = 366 * 500; i < n; ++i) {
        date d { i };
        EXPECT_EQ(d.year(), calendar->get(UCAL_YEAR, status));
        EXPECT_EQ(d.month(), calendar->get(UCAL_MONTH, status) + 1);
        EXPECT_EQ(d.day(), calendar->get(UCAL_DATE, status));
        calendar->add(UCAL_DATE, 1, status);
    }
}

TEST_F(date_datetime_test, triple) {
    for (std::uint32_t i = 0, n = 366 * 500; i < n; ++i) {
        date d1 { i };
        date d2 { d1.year(), d1.month(), d1.day() };
        EXPECT_EQ(d1, d2) << d1 << " : " << d2;
    }
}

TEST_F(date_datetime_test, output) {
    date d { 2019, 11, 15 };
    std::cout << d << std::endl;
}

} // namespace takatori::datetime
