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
 * @details This can represent about 592 years long since 1900-01-01.
 */
class time_point {
public:
    /// @brief time unit.
    using time_unit = std::chrono::duration<std::uint64_t, std::nano>;

    /// @brief time offset type.
    using difference_type = std::chrono::nanoseconds;

    /**
     * @brief creates a new instance of the epoch time, which represents 1900-01-01 00:00:00 GMT.
     */
    constexpr time_point() = default;

    /**
     * @brief creates a new instance
     * @param time_since_epoch elapsed time since 1900-01-01 00:00:00 GMT, ignoring leap seconds
     */
    explicit constexpr time_point(time_unit time_since_epoch) noexcept
        : elapsed_(time_since_epoch)
    {}


    /**
     * @brief creates a new instance from date and time.
     * @param date date in GMT
     * @param time time of the date
     */
    explicit time_point(datetime::date date, datetime::time_of_day time = {}) noexcept;

    /**
     * @brief creates a new instance.
     * @tparam Clock the clock type
     * @tparam Duration the duration type
     * @param time time system time point
     */
    template<class Clock, class Duration>
    explicit time_point(std::chrono::time_point<Clock, Duration> time)
        : elapsed_(from_chrono(std::chrono::time_point_cast<clock_unit>(time)))
    {}

    /**
     * @brief returns the time point of now in system clock.
     * @return time point of now
     */
    [[nodiscard]] static time_point now();

    /**
     * @brief returns the elapsed time since 1900-01-01 00:00:00 GMT, ignoring leap seconds.
     * @return the the elapsed time since the epoch
     */
    [[nodiscard]] constexpr time_unit time_since_epoch() const noexcept {
        return elapsed_;
    }

    /**
     * @brief returns the date of the time point (in GMT).
     * @return the date
     */
    [[nodiscard]] constexpr datetime::date date() const noexcept {
        using date_unit = std::chrono::duration<std::uint32_t, std::ratio<86'400>>;
        return datetime::date(std::chrono::floor<date_unit>(elapsed_).count());
    }

    /**
     * @brief returns the time in day of the time point (in GMT).
     * @return the time in day
     */
    [[nodiscard]] constexpr datetime::time_of_day time() const noexcept {
        return datetime::time_of_day(std::chrono::floor<datetime::time_of_day::time_unit>(elapsed_));
    }

    /**
     * @brief returns a pair of date and time of the time point (in GMT).
     * @return the date and time
     * @see calendar
     */
    [[nodiscard]] constexpr std::pair<datetime::date, datetime::time_of_day> date_time() const noexcept {
        return std::make_pair(date(), time());
    }

    /**
     * @brief adds offset into this time point.
     * @param offset the offset
     * @return this
     */
    constexpr time_point& operator+=(difference_type offset) noexcept;

    /**
     * @brief subtracts offset from this time point.
     * @param offset the offset
     * @return this
     */
    constexpr time_point& operator-=(difference_type offset) noexcept;

private:
    time_unit elapsed_ {};

    using clock_unit = std::chrono::nanoseconds;
    static time_unit from_chrono(std::chrono::time_point<std::chrono::system_clock, clock_unit> time);
};

/**
 * @brief adds a time point and offset.
 * @param a the time point
 * @param b the offset
 * @return the computed time point
 */
inline constexpr time_point operator+(time_point a, time_point::difference_type b) noexcept {
    return time_point { a.time_since_epoch() + b };
}

/**
 * @brief adds a time point and offset.
 * @param a the time point
 * @param b the offset
 * @return the computed time point
 */
inline constexpr time_point operator+(time_point::difference_type a, time_point b) noexcept {
    return b + a;
}

/**
 * @brief subtracts offset from the time point.
 * @param a the time point
 * @param b the offset
 * @return the computed time point
 */
inline constexpr time_point operator-(time_point a, time_point::difference_type b) noexcept {
    return a + -b;
}

/**
 * @brief returns difference between two time points.
 * @param a the first time point
 * @param b the second time point
 * @return the difference
 */
inline constexpr time_point::difference_type operator-(time_point a, time_point b) noexcept {
    return a.time_since_epoch() - b.time_since_epoch();
}

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
inline constexpr bool operator==(time_point a, time_point b) noexcept {
    return a.time_since_epoch() == b.time_since_epoch();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, time_point value);

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
inline constexpr bool operator!=(time_point a, time_point b) noexcept {
    return !(a == b);
}

constexpr time_point& time_point::operator+=(difference_type offset) noexcept {
    return *this = *this + offset;
}

constexpr time_point& time_point::operator-=(difference_type offset) noexcept {
    return *this = *this - offset;
}

} // namespace takatori::datetime

/// @brief provides hash code of takatori::datetime::time_point.
template<> struct std::hash<takatori::datetime::time_point> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::datetime::time_point object) const noexcept {
        return object.time_since_epoch().count() * 257;
    }
};
