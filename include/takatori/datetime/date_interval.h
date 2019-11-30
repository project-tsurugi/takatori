#pragma once

#include <functional>
#include <iostream>

#include <cstdint>

namespace takatori::datetime {

class datetime_interval;

/**
 * @brief represents date interval.
 * @note This is designed for the solar calendar.
 */
class date_interval {
public:
    /// @private the field unit type.
    using unit = std::int32_t;

    /**
     * @brief creates a new instance about 0-long date interval.
     */
    constexpr date_interval() = default;

    /**
     * @brief creates a new instance.
     * @param days day offset
     */
    explicit constexpr date_interval(unit days) noexcept;

    /**
     * @brief creates a new instance.
     * @param year years offset
     * @param month months offset
     * @param day days offset
     */
    constexpr date_interval(unit year, unit month, unit day) noexcept;

    /**
     * @brief returns offset of years.
     * @return offset of years
     */
    constexpr unit year() const noexcept;

    /**
     * @brief returns offset of months.
     * @return offset of months
     */
    constexpr unit month() const noexcept;

    /**
     * @brief returns offset of days.
     * @return offset of days
     */
    constexpr unit day() const noexcept;

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
    friend constexpr date_interval operator+(date_interval value) noexcept;

    /**
     * @brief negates the interval.
     * @param value the value
     * @return the negated value
     */
    friend constexpr date_interval operator-(date_interval value) noexcept;

    /**
     * @brief adds the two intervals.
     * @param a the first value
     * @param b the second value
     * @return the sum of two
     */
    friend constexpr date_interval operator+(date_interval a, date_interval b) noexcept;

    /**
     * @brief subtracts the second interval from the first one.
     * @param a the first value
     * @param b the second value
     * @return the difference of two
     */
    friend constexpr date_interval operator-(date_interval a, date_interval b) noexcept;

    /**
     * @brief adds the interval into this.
     * @param value the value
     * @return this
     */
    constexpr date_interval& operator+=(date_interval value) noexcept;

    /**
     * @brief subtracts the interval from this.
     * @param value the value
     * @return this
     */
    constexpr date_interval& operator-=(date_interval value) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(date_interval a, date_interval b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(date_interval a, date_interval b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, date_interval value);

private:
    unit years_ {};
    unit months_ {};
    unit days_ {};

    std::ostream& print_to(std::ostream& out);

    friend class datetime_interval;
};

inline constexpr date_interval::date_interval(unit days) noexcept
    : days_(days)
{}

inline constexpr date_interval::date_interval(unit year, unit month, unit day) noexcept
    : years_(year)
    , months_(month)
    , days_(day)
{}

inline constexpr date_interval::unit date_interval::year() const noexcept {
    return years_;
}

inline constexpr date_interval::unit date_interval::month() const noexcept {
    return months_;
}

inline constexpr date_interval::unit date_interval::day() const noexcept {
    return days_;
}

inline constexpr date_interval::operator bool() const noexcept {
    return years_ != 0
        || months_ != 0
        || days_ != 0;
}

inline constexpr date_interval operator+(date_interval value) noexcept {
    return value;
}

inline constexpr date_interval operator-(date_interval value) noexcept {
    return date_interval {
            -value.years_,
            -value.months_,
            -value.days_,
    };
}

inline constexpr date_interval operator+(date_interval a, date_interval b) noexcept {
    return date_interval {
            a.years_ + b.years_,
            a.months_ + b.months_,
            a.days_ + b.days_,
    };
}

inline constexpr date_interval operator-(date_interval a, date_interval b) noexcept {
    return date_interval {
            a.years_ - b.years_,
            a.months_ - b.months_,
            a.days_ - b.days_,
    };
}

inline constexpr date_interval& date_interval::operator+=(date_interval value) noexcept {
    return *this = *this + value;
}

inline constexpr date_interval& date_interval::operator-=(date_interval value) noexcept {
    return *this = *this - value;
}

inline constexpr bool operator==(date_interval a, date_interval b) noexcept {
    return a.years_ == b.years_
        && a.months_ == b.months_
        && a.days_ == b.days_;
}

inline constexpr bool operator!=(date_interval a, date_interval b) noexcept {
    return !(a == b);
}

} // namespace takatori::datetime

/// @brief provides hash code of takatori::datetime::date_interval.
template<> struct std::hash<takatori::datetime::date_interval> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::datetime::date_interval object) const noexcept {
        std::size_t result = 0;
        result = result * 31 + static_cast<std::size_t>(object.year());
        result = result * 31 + static_cast<std::size_t>(object.month());
        result = result * 257 + static_cast<std::size_t>(object.day());
        return result;
    }
};
