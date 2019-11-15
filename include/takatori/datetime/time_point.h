#pragma once

#include <functional>
#include <iostream>
#include <utility>

#include <cstdint>

#include "date.h"
#include "time_of_day.h"

namespace takatori::datetime {

/**
 * @brief represents time point since 1900-01-01 00:00:00 GMT.
 * This can represent about 592 years long since 1900-01-01.
 */
class time_point {
public:
    /// @brief time unit.
    using time_unit = std::chrono::duration<std::uint64_t, std::nano>;

    /**
     * @brief creates a new instance of the epoch time, which represents 1900-01-01 00:00:00 GMT.
     */
    constexpr time_point() = default;

    /**
     * @brief creates a new instance
     * @param elapsed elapsed time since 1900-01-01 00:00:00 GMT, ignoring leap seconds
     */
    explicit constexpr time_point(time_unit elapsed) noexcept;

    /**
     * @brief creates a new instance from date and time.
     * @param date date in GMT
     * @param time time of the date
     */
    explicit time_point(datetime::date date, datetime::time_of_day time = {}) noexcept;

    /**
     * @brief creates a new instance from date and time.
     * @param date date in GMT
     * @param time time of the date
     */
    template<class Clock, class Duration>
    explicit time_point(std::chrono::time_point<Clock, Duration> time);

    /**
     * @brief returns the time point of now in system clock.
     * @return now
     */
    static time_point now();

    /**
     * @brief returns the elapsed time since 1900-01-01 00:00:00 GMT, ignoring leap seconds.
     * @return the the elapsed time since the epoch
     */
    constexpr time_unit elapsed() const noexcept;

    /**
     * @brief returns the date of the time point (in GMT).
     * @return the date
     */
    constexpr datetime::date date() const noexcept;

    /**
     * @brief returns the time in day of the time point (in GMT).
     * @return the time in day
     */
    constexpr datetime::time_of_day time() const noexcept;

    /**
     * @brief returns a pair of date and time of the time point (in GMT).
     * @return the date and time
     */
    constexpr std::pair<datetime::date, datetime::time_of_day> date_time() const noexcept;

    // FIXME: with tz

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(time_point const& a, time_point const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(time_point const& a, time_point const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_point const& value);

private:
    time_unit elapsed_ {};

    using clock_unit = std::chrono::nanoseconds;
    static time_unit from_chrono(std::chrono::time_point<std::chrono::system_clock, clock_unit> time);

    std::ostream& print_body(std::ostream& out) const;
};

constexpr time_point::time_point(time_unit elapsed) noexcept
    : elapsed_(elapsed)
{}

template<class Clock, class Duration>
time_point::time_point(std::chrono::time_point<Clock, Duration> time)
    : elapsed_(from_chrono(std::chrono::time_point_cast<clock_unit>(time)))
{}

inline constexpr time_point::time_unit time_point::elapsed() const noexcept {
    return elapsed_;
}

constexpr datetime::date time_point::date() const noexcept {
    using date_unit = std::chrono::duration<std::uint32_t, std::ratio<86'400>>;
    return datetime::date(std::chrono::floor<date_unit>(elapsed_).count());
}

constexpr datetime::time_of_day time_point::time() const noexcept {
    return datetime::time_of_day(std::chrono::floor<datetime::time_of_day::time_unit>(elapsed_));
}

constexpr std::pair<datetime::date, datetime::time_of_day> time_point::date_time() const noexcept {
    return std::make_pair(date(), time());
}

inline constexpr bool operator==(time_point const& a, time_point const& b) noexcept {
    return a.elapsed_ == b.elapsed_;
}

inline constexpr bool operator!=(time_point const& a, time_point const& b) noexcept {
    return !(a == b);
}

} // namespace takatori::datetime

/// @brief provides hash code of takatori::datetime::time_point.
template<> struct std::hash<takatori::datetime::time_point> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::datetime::time_point const& object) const noexcept {
        return object.elapsed().count() * 257;
    }
};
