#pragma once

#include <chrono>
#include <functional>
#include <iostream>

#include <cstdint>

namespace takatori::datetime {

/**
 * @brief represents time interval.
 */
class time_interval {
public:
    /**
     * @brief creates a new instance about 0-long time interval.
     */
    constexpr time_interval() = default;

    /**
     * @brief creates a new instance.
     * @param months the interval months
     * @param nanoseconds the interval nano-seconds
     */
    explicit constexpr time_interval(std::int64_t months, std::int64_t nanoseconds) noexcept;

    /**
     * @brief creates a new instance.
     * @tparam Rep the representation type
     * @tparam Period the tick period
     * @param duration the duration
     */
    template<class Rep, class Period>
    constexpr time_interval(std::chrono::duration<Rep, Period> duration) noexcept; // NOLINT

    /**
     * @brief returns the total interval months.
     * @return the total interval months
     */
    constexpr std::int64_t total_months() const noexcept;

    /**
     * @brief returns the total interval time in nano-second.
     * This does not include total interval months field.
     * @return the total interval time in nano-second
     */
    constexpr std::int64_t total_nanoseconds() const noexcept;

    /**
     * @brief returns itself.
     * @param value the value
     * @return the value
     */
    friend constexpr time_interval operator+(time_interval const& value) noexcept;

    /**
     * @brief negates the interval.
     * @param value the value
     * @return the negated value
     */
    friend constexpr time_interval operator-(time_interval const& value) noexcept;

    /**
     * @brief adds the two intervals.
     * @param a the first value
     * @param b the second value
     * @return the sum of two
     */
    friend constexpr time_interval operator+(time_interval const& a, time_interval const& b) noexcept;

    /**
     * @brief subtracts the second interval from the first one.
     * @param a the first value
     * @param b the second value
     * @return the difference of two
     */
    friend constexpr time_interval operator-(time_interval const& a, time_interval const& b) noexcept;

    /**
     * @brief adds the interval into this.
     * @param value the value
     * @return this
     */
    constexpr time_interval& operator+=(time_interval const& value) noexcept;

    /**
     * @brief subtracts the interval from this.
     * @param value the value
     * @return this
     */
    constexpr time_interval& operator-=(time_interval const& value) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(time_interval const& a, time_interval const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(time_interval const& a, time_interval const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_interval const& value);

private:
    std::int64_t total_months_ {};
    std::int64_t total_nanoseconds_ {};
};

inline constexpr time_interval::time_interval(std::int64_t months, std::int64_t nanoseconds) noexcept
    : total_months_(months)
    , total_nanoseconds_(nanoseconds)
{}

template<class Rep, class Period>
inline constexpr time_interval::time_interval(std::chrono::duration<Rep, Period> duration) noexcept
    : total_nanoseconds_(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count())
{}

inline constexpr std::int64_t time_interval::total_months() const noexcept {
    return total_months_;
}

inline constexpr std::int64_t time_interval::total_nanoseconds() const noexcept {
    return total_nanoseconds_;
}

constexpr time_interval operator+(time_interval const& value) noexcept {
    return value;
}

constexpr time_interval operator-(time_interval const& value) noexcept {
    return time_interval(
            -value.total_months_,
            -value.total_nanoseconds_);
}

constexpr time_interval operator+(time_interval const& a, time_interval const& b) noexcept {
    return time_interval(
            a.total_months_ + b.total_months_,
            a.total_nanoseconds_ + b.total_nanoseconds_);
}

constexpr time_interval operator-(time_interval const& a, time_interval const& b) noexcept {
    return time_interval(
            a.total_months_ - b.total_months_,
            a.total_nanoseconds_ - b.total_nanoseconds_);
}

constexpr time_interval& time_interval::operator+=(time_interval const& value) noexcept {
    return *this = *this + value;
}

constexpr time_interval& time_interval::operator-=(time_interval const& value) noexcept {
    return *this = *this - value;
}

inline constexpr bool operator==(time_interval const& a, time_interval const& b) noexcept {
    return a.total_months_ == b.total_months_
        && a.total_nanoseconds_ == b.total_nanoseconds_;
}

inline constexpr bool operator!=(time_interval const& a, time_interval const& b) noexcept {
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
    constexpr std::size_t operator()(takatori::datetime::time_interval const& object) const noexcept {
        return static_cast<std::size_t>(object.total_months()) * 31
               + static_cast<std::size_t>(object.total_nanoseconds());
    }
};
