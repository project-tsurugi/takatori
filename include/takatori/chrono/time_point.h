#pragma once

#include <functional>
#include <iostream>

#include <cstdint>

#include "date.h"
#include "time_of_day.h"

namespace takatori::chrono {

/**
 * @brief represents time point since 1900-01-01 00:00:00 GMT.
 */
class time_point {
public:
    /**
     * @brief creates a new instance which represents 1900-01-01 00:00:00 GMT.
     */
    constexpr time_point() = default;

    /**
     * @brief creates a new instance.
     * @param date the date
     * @param time the time of day
     */
    explicit constexpr time_point(class date date, class time_of_day time) noexcept;

    /**
     * @brief returns the date.
     * @return the date
     */
    constexpr class date date() const noexcept;

    /**
     * @brief returns the time of day.
     * @return the time of day
     */
    constexpr class time_of_day time() const noexcept;

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
    class date date_ {};
    class time_of_day time_ {};

    std::ostream& print_body(std::ostream& out) const;
};

constexpr time_point::time_point(class date date, class time_of_day time) noexcept
    : date_(date)
    , time_(time)
{}

constexpr class date time_point::date() const noexcept {
    return date_;
}

constexpr class time_of_day time_point::time() const noexcept {
    return time_;
}

inline constexpr bool operator==(time_point const& a, time_point const& b) noexcept {
    return a.date() == b.date()
        && a.time() == b.time();
}

inline constexpr bool operator!=(time_point const& a, time_point const& b) noexcept {
    return !(a == b);
}

} // namespace takatori::chrono

/// @brief provides hash code of takatori::chrono::time_point.
template<> struct std::hash<takatori::chrono::time_point> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::chrono::time_point const& object) const noexcept {
        return ::std::hash<takatori::chrono::date>{}(object.date()) * 31
            + ::std::hash<takatori::chrono::time_of_day>{}(object.time());
    }
};
