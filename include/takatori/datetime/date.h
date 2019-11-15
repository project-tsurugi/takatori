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
    explicit constexpr date(std::uint32_t days_since_epoch) noexcept;

    /**
     * @brief creates a new instance from year. month, and day triple.
     * @param year the year (1900-)
     * @param month the month number (1-12) of year
     * @param day the day (1-31) of month
     */
    explicit date(std::uint32_t year, std::uint32_t month, std::uint32_t day) noexcept;

    /**
     * @brief returns the elapsed days since 1900-01-01.
     * @return the the elapsed days
     */
    constexpr std::uint32_t days_since_epoch() const noexcept;

    /**
     * @brief returns the year (1900-).
     * @return the year
     */
    std::uint32_t year() const noexcept;

    /**
     * @brief returns the month number (1-12) of the year.
     * @return the month number
     */
    std::uint32_t month() const noexcept;

    /**
     * @brief returns the day of the month.
     * @return the day of month
     */
    std::uint32_t day() const noexcept;

    /**
     * @brief adds days with the date.
     * @param a the date
     * @param b the day offset
     * @return the computed date
     */
    friend constexpr date operator+(date a, difference_type b) noexcept;

    /**
     * @brief adds days with the date.
     * @param a the day offset
     * @param b the date
     * @return the computed date
     */
    friend constexpr date operator+(difference_type a, date b) noexcept;

    /**
     * @brief subtracts days from the date.
     * @param a the date
     * @param b the day offset
     * @return the computed date
     */
    friend constexpr date operator-(date a, difference_type b) noexcept;

    /**
     * @brief returns difference between two dates.
     * @param a the first date
     * @param b the second date
     * @return the duration in days
     */
    friend constexpr difference_type operator-(date a, date b) noexcept;

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

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(date a, date b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(date a, date b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, date value);

private:
    std::uint32_t elapsed_ {};

    std::ostream& print_body(std::ostream& out) const;

    friend class time_point;
};

inline constexpr date::date(std::uint32_t days_since_epoch) noexcept
    : elapsed_(days_since_epoch)
{}

inline constexpr std::uint32_t date::days_since_epoch() const noexcept {
    return elapsed_;
}

constexpr date operator+(date a, date::difference_type b) noexcept {
    return date { a.elapsed_ + b };
}

constexpr date operator+(date::difference_type a, date b) noexcept {
    return b + a;
}

constexpr date operator-(date a, date::difference_type b) noexcept {
    return a + -b;
}

constexpr date::difference_type operator-(date a, date b) noexcept {
    return a.elapsed_ - b.elapsed_;
}

constexpr date& date::operator+=(difference_type offset) noexcept {
    return *this = *this + offset;
}

constexpr date& date::operator-=(difference_type offset) noexcept {
    return *this = *this - offset;
}

inline constexpr bool operator==(date a, date b) noexcept {
    return a.elapsed_ == b.elapsed_;
}

inline constexpr bool operator!=(date a, date b) noexcept {
    return !(a == b);
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
