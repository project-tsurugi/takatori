#pragma once

#include <chrono>
#include <functional>
#include <iostream>

#include <cstdint>

namespace takatori::datetime {

class time_point;

/**
 * @brief represents date since 1900-01-01 on gregorian calendar.
 */
class date {
public:
    /// @brief date count type.
    using size_type = std::uint32_t;

    /// @brief date offset type.
    using difference_type = std::int32_t;

    /**
     * @brief creates a new instance which represents 1900-01-01.
     */
    constexpr date() = default;

    /**
     * @brief creates a new instance.
     * @param days_since_epoch the elapsed days since 1900-01-01
     */
    explicit constexpr date(size_type days_since_epoch) noexcept
        : elapsed_(days_since_epoch)
    {}

    /**
     * @brief creates a new instance from year. month, and day triple.
     * @param year the year (1900-)
     * @param month the month number (1-12) of year
     * @param day the day (1-31) of month
     */
    explicit date(size_type year, size_type month, size_type day) noexcept;

    /**
     * @brief returns the elapsed days since 1900-01-01.
     * @return the the elapsed days
     */
    constexpr size_type days_since_epoch() const noexcept {
        return elapsed_;
    }

    /**
     * @brief returns the year (1900-).
     * @return the year
     */
    size_type year() const noexcept;

    /**
     * @brief returns the month number (1-12) of the year.
     * @return the month number
     */
    size_type month() const noexcept;

    /**
     * @brief returns the day of the month.
     * @return the day of month
     */
    size_type day() const noexcept;

    /**
     * @brief adds offset into this date.
     * @param offset the day offset
     * @return this
     */
    constexpr date& operator+=(difference_type offset) noexcept;

    /**
     * @brief subtracts offset from this date.
     * @param offset the day offset
     * @return this
     */
    constexpr date& operator-=(difference_type offset) noexcept;

private:
    size_type elapsed_ {};
};

/**
 * @brief adds days with the date.
 * @param a the date
 * @param b the day offset
 * @return the computed date
 */
inline constexpr date operator+(date a, date::difference_type b) noexcept {
    return date { a.days_since_epoch() + b };
}

/**
 * @brief adds days with the date.
 * @param a the day offset
 * @param b the date
 * @return the computed date
 */
inline constexpr date operator+(date::difference_type a, date b) noexcept {
    return b + a;
}

/**
 * @brief subtracts days from the date.
 * @param a the date
 * @param b the day offset
 * @return the computed date
 */
inline constexpr date operator-(date a, date::difference_type b) noexcept {
    return a + -b;
}

/**
 * @brief returns difference between two dates.
 * @param a the first date
 * @param b the second date
 * @return the duration in days
 */
inline constexpr date::difference_type operator-(date a, date b) noexcept {
    return a.days_since_epoch() - b.days_since_epoch();
}

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
inline constexpr bool operator==(date a, date b) noexcept {
    return a.days_since_epoch() == b.days_since_epoch();
}

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
inline constexpr bool operator!=(date a, date b) noexcept {
    return !(a == b);
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, date value);

constexpr date& date::operator+=(date::difference_type offset) noexcept {
    return *this = *this + offset;
}

constexpr date& date::operator-=(date::difference_type offset) noexcept {
    return *this = *this - offset;
}

} // namespace takatori::datetime

/// @brief provides hash code of takatori::datetime::date.
template<> struct std::hash<takatori::datetime::date> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::datetime::date object) const noexcept {
        return object.days_since_epoch();
    }
};
