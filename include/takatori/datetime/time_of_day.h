#pragma once

#include <chrono>
#include <functional>
#include <iostream>

#include <cstdint>

namespace takatori::datetime {

class time_point;

/**
 * @brief represents time of day.
 */
class time_of_day {
public:
    /// @brief time unit.
    using time_unit = std::chrono::duration<std::uint64_t, std::nano>;

    /// @brief unit of subsecond field.
    using subsecond_unit = std::chrono::duration<std::uint32_t, std::nano>;

    /**
     * @brief creates a new instance which represents 00:00:00 of day.
     */
    constexpr time_of_day() = default;

    /**
     * @brief creates a new instance.
     * @param elapsed the elapsed time since 00:00:00 of day
     */
    explicit constexpr time_of_day(time_unit elapsed) noexcept;

    /**
     * @brief creates a new instance.
     * @param hour hour of day
     * @param minute minute of hour
     * @param second second of minute
     * @param subsecond sub-second value
     */
    explicit time_of_day(
            std::uint32_t hour,
            std::uint32_t minute,
            std::uint32_t second,
            time_unit subsecond = time_unit::zero()) noexcept;

    /**
     * @brief returns the elapsed time since 00:00:00 of day.
     * @return the the elapsed time since 00:00:00
     */
    constexpr time_unit elapsed() const noexcept;

    /**
     * @brief returns the hour of day (0-23)
     * @return the hour of day
     */
    std::uint32_t hour() const noexcept;

    /**
     * @brief returns the minute of hour (0-59)
     * @return the minute of hour
     */
    std::uint32_t minute() const noexcept;

    /**
     * @brief returns the second of minute (0-59)
     * @return the second of hour
     */
    std::uint32_t second() const noexcept;

    /**
     * @brief returns the sub-second value.
     * @return the sub-second value
     */
    subsecond_unit subsecond() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(time_of_day const& a, time_of_day const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(time_of_day const& a, time_of_day const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_of_day const& value);

private:
    time_unit elapsed_ {};

    static constexpr time_unit normalize(time_unit time) noexcept {
        constexpr std::uint64_t day = 86'400 * static_cast<std::uint64_t>(time_unit::period::den);
        return time_unit(time.count() % day);
    }

    std::ostream& print_body(std::ostream& out) const;

    friend class time_point;
};

constexpr time_of_day::time_of_day(time_unit elapsed) noexcept
    : elapsed_(normalize(elapsed))
{}

inline constexpr time_of_day::time_unit time_of_day::elapsed() const noexcept {
    return elapsed_;
}

inline constexpr bool operator==(time_of_day const& a, time_of_day const& b) noexcept {
    return a.elapsed_ == b.elapsed_;
}

inline constexpr bool operator!=(time_of_day const& a, time_of_day const& b) noexcept {
    return !(a == b);
}

} // namespace takatori::datetime

/// @brief provides hash code of takatori::datetime::time_of_day.
template<> struct std::hash<takatori::datetime::time_of_day> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::datetime::time_of_day const& object) const noexcept {
        return object.elapsed().count();
    }
};
