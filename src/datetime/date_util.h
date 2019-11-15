#pragma once

#include <array>

#include <cstdint>

namespace takatori::datetime::util {

/**
 * @brief year and day of year.
 */
struct year_and_day {
    std::uint32_t year;
    std::uint32_t day_of_year;
};

/**
 * @brief month and day of month.
 */
struct month_and_day {
    std::uint32_t month;
    std::uint32_t day_of_month;
};

constexpr std::uint64_t days_year = 365;

constexpr std::uint32_t days_january = 31;
constexpr std::uint32_t days_february = days_january + 28;
constexpr std::uint32_t days_march = days_february + 31;
constexpr std::uint32_t days_april = days_march + 30;
constexpr std::uint32_t days_may = days_april + 31;
constexpr std::uint32_t days_june = days_may + 30;
constexpr std::uint32_t days_july = days_june + 31;
constexpr std::uint32_t days_august = days_july + 31;
constexpr std::uint32_t days_september = days_august + 30;
constexpr std::uint32_t days_october = days_september + 31;
constexpr std::uint32_t days_november = days_october + 30;

// constants for compute leap year

/// @brief year of internal epoch - 0001/01/01: easy to compute leap years
constexpr std::int32_t internal_epoch_year = 1;

/// @brief elapsed days from 0001/01/01 to 1900/01/01
constexpr std::uint64_t public_epoch_day_since_internal = 693'595;

// leap year comes every 4 years (leap tick)
/// @brief years in leap tick
constexpr std::uint32_t years_leap_tick = 4;

/// @brief days in leap tick
constexpr std::uint64_t days_leap_tick =
        days_year * years_leap_tick
        + (years_leap_tick / 4)
        - (years_leap_tick / 100)
        + (years_leap_tick / 400);

// every 100 years, leap year does not come even if it is leap tick (century)
/// @brief years in century
constexpr std::uint32_t years_century = 100;

/// @brief days in century
constexpr std::uint64_t days_century =
        days_year * years_century
        + (years_century / 4)
        - (years_century / 100)
        + (years_century / 400);

// every 400 years, leap year comes even if it is century (leap cycle)

/// @brief years in leap cycle
constexpr std::uint32_t years_leap_cycle = 400;

/// @brief days in leap cycle
constexpr std::uint64_t days_leap_cycle =
        days_year * years_leap_cycle
        + (years_leap_cycle / 4)
        - (years_leap_cycle / 100)
        + (years_leap_cycle / 400);

/**
 * @brief returns whether or not the target year is leap.
 * @param year the target year
 * @return true if the target year is leap
 * @return false otherwise
 */
constexpr bool is_leap(std::uint32_t year) {
    if (year % 4 != 0) {
        return false;
    }
    return (year % 100) != 0 || (year % 400) == 0;
}

/**
 * @brief compute days since epoch (1900-01-01) from the the year.
 * @param year the year
 * @return the days since epoch
 */
constexpr std::uint32_t to_days_since_epoch(std::uint32_t year) noexcept {
    std::uint32_t y = year - 1;
    std::uint64_t days_since_internal_epoch = days_year * y + (y / 4) - (y / 100) + (y / 400);
    return static_cast<std::uint32_t>(days_since_internal_epoch - public_epoch_day_since_internal);
}

/**
 * @brief compute days since epoch (1900-01-01) from the given date triple.
 * @param year the year
 * @param month the month of year
 * @param day the day of month
 * @return the days since epoch
 */
constexpr std::uint32_t to_days_since_epoch(std::uint32_t year, std::uint32_t month, std::uint32_t day) noexcept {
    constexpr std::array<std::uint32_t, 12> days_begin_of_month {
            0,
            days_january,
            days_february,
            days_march,
            days_april,
            days_may,
            days_june,
            days_july,
            days_august,
            days_september,
            days_october,
            days_november,
    };
    std::uint32_t result = to_days_since_epoch(year);
    result += days_begin_of_month[std::min(month - 1, 11U)]; // NOLINT
    result += day - 1;
    if (month >= 3 && is_leap(year)) result += 1;
    return result;
}

/**
 * @brief extracts year and day from days since epoch - 1900/01/01.
 * @param days the number of elapsed days
 * @return the corresponded year
 */
constexpr year_and_day to_year_and_day(std::uint32_t days) {
    // we use internal epoch 0001/01/01 to simplify to compute leap years
    std::uint64_t days_since_internal_epoch = days + public_epoch_day_since_internal;

    // the number of leap year cycles (400years)
    std::uint64_t cycles = days_since_internal_epoch / days_leap_cycle;
    std::uint64_t cycle_rest = days_since_internal_epoch % days_leap_cycle;

    // the century offset in the current leap year cycle (0-3)
    std::uint64_t cent_in_cycle = cycle_rest / days_century;
    std::uint64_t cent_rest = cycle_rest % days_century;
    cent_rest += days_century * (cent_in_cycle / (years_leap_cycle / years_century));
    cent_in_cycle -= (cent_in_cycle / (years_leap_cycle / years_century));

    // the leap tick offset in the current century (0-24)
    std::uint64_t tick_in_cent = cent_rest / days_leap_tick;
    std::uint64_t tick_rest = cent_rest % days_leap_tick;

    // the year offset since the last leap tick (0-3)
    std::uint64_t year_in_tick = tick_rest / days_year;
    year_in_tick -= (year_in_tick / years_leap_tick);

    // compute the year and day of year
    std::uint64_t year =
            internal_epoch_year
            + years_leap_cycle * cycles
            + years_century * cent_in_cycle
            + years_leap_tick * tick_in_cent
            + year_in_tick;
    return {
        static_cast<std::uint32_t>(year),
        days - to_days_since_epoch(year),
    };
}

/**
 * @brief extract month and day from the day of year.
 * @param year the year number
 * @param day_of_year the number of elapsed days from beginning of the year
 * @return a pair of the month number and day in the month
 */
constexpr month_and_day to_month_and_day(std::uint32_t year, std::uint32_t day_of_year) {
    std::uint32_t d = day_of_year;
    if (d < days_january)   return {  1, d + 1 };
    if (d < days_february)  return {  2, d - days_january + 1 };
    if (is_leap(year)) {
        if (d == days_february) return { 2, 29 };
        --d;
    }
    if (d < days_march)     return {  3, d - days_february + 1 };
    if (d < days_april)     return {  4, d - days_march + 1 };
    if (d < days_may)       return {  5, d - days_april + 1 };
    if (d < days_june)      return {  6, d - days_may + 1 };
    if (d < days_july)      return {  7, d - days_june + 1 };
    if (d < days_august)    return {  8, d - days_july + 1 };
    if (d < days_september) return {  9, d - days_august + 1 };
    if (d < days_october)   return { 10, d - days_september + 1 };
    if (d < days_november)  return { 11, d - days_october + 1 };
    return { 12, d - days_november + 1 };
}

} // namespace takatori::datetime::util
