#pragma once

#include <chrono>
#include <functional>
#include <iostream>

#include <cstdint>

namespace takatori::datetime {

class datetime_interval;

/**
 * @brief represents time interval.
 */
class time_interval {
public:
    /// @private the time unit.
    using time_unit = std::chrono::nanoseconds;

    /// @brief unit of subsecond field.
    using subsecond_unit = std::chrono::duration<std::int32_t, std::nano>;

    /**
     * @brief creates a new instance about 0-long date interval.
     */
    constexpr time_interval() = default;

    /**
     * @brief creates a new instance.
     * @param offset the time offset
     */
    explicit constexpr time_interval(time_unit offset) noexcept;

    /**
     * @brief creates a new instance.
     * @param hour hours offset
     * @param minute minutes offset
     * @param second seconds offset
     * @param subsecond sub-second offset
     */
    constexpr time_interval(
            std::int32_t hour,
            std::int32_t minute,
            std::int32_t second,
            time_unit subsecond = time_unit::zero()) noexcept;

    /**
     * @brief returns the time offset.
     * @return the time offset
     */
    constexpr time_unit offset() const noexcept;

    /**
     * @brief returns the interval of days.
     * @return interval of days
     */
    constexpr std::int32_t day() const noexcept;

    /**
     * @brief returns the interval of hours in day - (-24, +24).
     * @return interval of hours in day
     */
    constexpr std::int32_t hour() const noexcept;

    /**
     * @brief returns the interval of minutes in hour - (-60, +60).
     * @return interval of minutes in hour
     */
    constexpr std::int32_t minute() const noexcept;

    /**
     * @brief returns the interval of seconds in minute - (-60, +60).
     * @return interval of seconds in minute
     */
    constexpr std::int32_t second() const noexcept;

    /**
     * @brief returns the interval of sub-second in second (-1, +1).
     * @return interval of sub-second value
     */
    constexpr subsecond_unit subsecond() const noexcept;

    /**
     * @brief returns whether or not this is non-zero interval.
     * @return true if this is non-zero
     * @return false otherwise
     */
    explicit constexpr operator bool() const noexcept;

    /**
     * @brief returns itself.
     * @param value the value
     * @return the value
     */
    friend constexpr time_interval operator+(time_interval value) noexcept;

    /**
     * @brief negates the interval.
     * @param value the value
     * @return the negated value
     */
    friend constexpr time_interval operator-(time_interval value) noexcept;

    /**
     * @brief adds the two intervals.
     * @param a the first value
     * @param b the second value
     * @return the sum of two
     */
    friend constexpr time_interval operator+(time_interval a, time_interval b) noexcept;

    /**
     * @brief subtracts the second interval from the first one.
     * @param a the first value
     * @param b the second value
     * @return the difference of two
     */
    friend constexpr time_interval operator-(time_interval a, time_interval b) noexcept;

    /**
     * @brief adds the interval into this.
     * @param value the value
     * @return this
     */
    constexpr time_interval& operator+=(time_interval value) noexcept;

    /**
     * @brief subtracts the interval from this.
     * @param value the value
     * @return this
     */
    constexpr time_interval& operator-=(time_interval value) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(time_interval a, time_interval b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(time_interval a, time_interval b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_interval value);

private:
    time_unit offset_ {};

    std::ostream& print_to(std::ostream& out);

    friend class datetime_interval;
};

inline constexpr time_interval::time_interval(time_interval::time_unit offset) noexcept
    : offset_(offset)
{}

inline constexpr time_interval::time_interval(
        std::int32_t hour,
        std::int32_t minute,
        std::int32_t second,
        time_unit subsecond) noexcept
    : offset_(std::chrono::hours(hour)
            + std::chrono::minutes(minute)
            + std::chrono::seconds(second)
            + subsecond)
{}

inline constexpr time_interval::time_unit time_interval::offset() const noexcept {
    return offset_;
}

inline constexpr std::int32_t time_interval::day() const noexcept {
    using unit = std::chrono::duration<std::int64_t, std::ratio<86'400>>;
    return static_cast<std::int32_t>(std::chrono::duration_cast<unit>(offset_).count());
}

inline constexpr std::int32_t time_interval::hour() const noexcept {
    using unit = std::chrono::duration<std::int64_t, std::chrono::hours::period>;
    return static_cast<std::int32_t>(std::chrono::duration_cast<unit>(offset_).count() % 24);
}

inline constexpr std::int32_t time_interval::minute() const noexcept {
    using unit = std::chrono::duration<std::int64_t, std::chrono::minutes::period>;
    return static_cast<std::int32_t>(std::chrono::duration_cast<unit>(offset_).count() % 60);
}

inline constexpr std::int32_t time_interval::second() const noexcept {
    using unit = std::chrono::duration<std::int64_t, std::chrono::seconds::period>;
    return static_cast<std::int32_t>(std::chrono::duration_cast<unit>(offset_).count() % 60);
}

inline constexpr time_interval::subsecond_unit time_interval::subsecond() const noexcept {
    using unit = time_interval::subsecond_unit;
    return unit { offset_.count() % static_cast<unit::rep>(std::nano::den) };
}

inline constexpr time_interval::operator bool() const noexcept {
    return offset_.count() != 0;
}

inline constexpr time_interval operator+(time_interval value) noexcept {
    return value;
}

inline constexpr time_interval operator-(time_interval value) noexcept {
    return time_interval { -value.offset_ };
}

inline constexpr time_interval operator+(time_interval a, time_interval b) noexcept {
    return time_interval { a.offset_ + b.offset_ };
}

inline constexpr time_interval operator-(time_interval a, time_interval b) noexcept {
    return time_interval { a.offset_ - b.offset_ };
}

inline constexpr time_interval& time_interval::operator+=(time_interval value) noexcept {
    return *this = *this + value;
}

inline constexpr time_interval& time_interval::operator-=(time_interval value) noexcept {
    return *this = *this - value;
}

inline constexpr bool operator==(time_interval a, time_interval b) noexcept {
    return a.offset_ == b.offset_;
}

inline constexpr bool operator!=(time_interval a, time_interval b) noexcept {
    return !(a == b);
}

} // namespace takatori::datetime

/// @brief provides hash code of takatori::datetime::time_interval.
template<> struct std::hash<takatori::datetime::time_interval> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::datetime::time_interval object) const noexcept {
        return object.offset().count();
    }
};
