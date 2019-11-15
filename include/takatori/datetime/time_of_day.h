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

    /// @brief time offset type.
    using difference_type = std::chrono::nanoseconds;

    /**
     * @brief creates a new instance which represents 00:00:00 of day.
     */
    constexpr time_of_day() = default;

    /**
     * @brief creates a new instance.
     * @param time_since_epoch the elapsed time since 00:00:00 of day
     */
    explicit constexpr time_of_day(time_unit time_since_epoch) noexcept;

    /**
     * @brief creates a new instance.
     * @param hour hour of day
     * @param minute minute of hour
     * @param second second of minute
     * @param subsecond sub-second value
     */
    constexpr time_of_day(
            std::uint32_t hour,
            std::uint32_t minute,
            std::uint32_t second,
            time_unit subsecond = time_unit::zero()) noexcept;

    /**
     * @brief returns the elapsed time since 00:00:00 of day.
     * @return the the elapsed time since 00:00:00
     */
    constexpr time_unit time_since_epoch() const noexcept;

    /**
     * @brief returns the hour of day (0-23)
     * @return the hour of day
     */
    constexpr std::uint32_t hour() const noexcept;

    /**
     * @brief returns the minute of hour (0-59)
     * @return the minute of hour
     */
    constexpr std::uint32_t minute() const noexcept;

    /**
     * @brief returns the second of minute (0-59)
     * @return the second of hour
     */
    constexpr std::uint32_t second() const noexcept;

    /**
     * @brief returns the sub-second value.
     * @return the sub-second value
     */
    constexpr subsecond_unit subsecond() const noexcept;

    /**
     * @brief adds a time and offset.
     * @param a the time
     * @param b the offset
     * @return the computed time
     */
    friend constexpr time_of_day operator+(time_of_day a, difference_type b) noexcept;

    /**
     * @brief adds a time and offset.
     * @param a the time
     * @param b the offset
     * @return the computed time
     */
    friend constexpr time_of_day operator+(difference_type a, time_of_day b) noexcept;

    /**
     * @brief subtracts offset from the time.
     * @param a the time
     * @param b the offset
     * @return the computed time
     */
    friend constexpr time_of_day operator-(time_of_day a, difference_type b) noexcept;

    /**
     * @brief returns difference between two times.
     * @param a the first time
     * @param b the second time
     * @return the difference
     */
    friend constexpr difference_type operator-(time_of_day a, time_of_day b) noexcept;

    /**
     * @brief adds offset into this time.
     * @param offset the offset
     * @return this
     */
    constexpr time_of_day& operator+=(difference_type offset) noexcept;

    /**
     * @brief subtracts offset from this time.
     * @param offset the offset
     * @return this
     */
    constexpr time_of_day& operator-=(difference_type offset) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(time_of_day a, time_of_day b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(time_of_day a, time_of_day b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_of_day value);

private:
    time_unit elapsed_ {};

    static constexpr time_unit normalize(time_unit time) noexcept {
        constexpr std::uint64_t day = 86'400 * static_cast<std::uint64_t>(time_unit::period::den);
        return time_unit(time.count() % day);
    }

    std::ostream& print_body(std::ostream& out) const;

    friend class time_point;
};

inline constexpr time_of_day::time_of_day(time_unit time_since_epoch) noexcept
    : elapsed_(normalize(time_since_epoch))
{}

inline constexpr time_of_day::time_of_day(
        std::uint32_t hour,
        std::uint32_t minute,
        std::uint32_t second,
        time_of_day::time_unit subsecond) noexcept
    : elapsed_(
        std::chrono::duration<std::uint64_t, std::chrono::hours::period>(hour)
        + std::chrono::duration<std::uint64_t, std::chrono::minutes::period>(minute)
        + std::chrono::duration<std::uint64_t, std::chrono::seconds::period>(second)
        + subsecond)
{}

inline constexpr time_of_day::time_unit time_of_day::time_since_epoch() const noexcept {
    return elapsed_;
}

inline constexpr std::uint32_t time_of_day::hour() const noexcept {
    using unit = std::chrono::duration<std::uint64_t, std::chrono::hours::period>;
    return static_cast<std::uint32_t>(std::chrono::duration_cast<unit>(elapsed_).count());
}

inline constexpr std::uint32_t time_of_day::minute() const noexcept {
    using unit = std::chrono::duration<std::uint64_t, std::chrono::minutes::period>;
    return static_cast<std::uint32_t>(std::chrono::duration_cast<unit>(elapsed_).count() % 60);
}

inline constexpr std::uint32_t time_of_day::second() const noexcept {
    using unit = std::chrono::duration<std::uint64_t, std::chrono::seconds::period>;
    return static_cast<std::uint32_t>(std::chrono::duration_cast<unit>(elapsed_).count() % 60);
}

inline constexpr time_of_day::subsecond_unit time_of_day::subsecond() const noexcept {
    using unit = subsecond_unit;
    return unit { elapsed_.count() % static_cast<unit::rep>(std::nano::den) };
}

inline constexpr time_of_day operator+(time_of_day a, time_of_day::difference_type b) noexcept {
    return time_of_day { a.elapsed_ + b };
}

inline constexpr time_of_day operator+(time_of_day::difference_type a, time_of_day b) noexcept {
    return b + a;
}

inline constexpr time_of_day operator-(time_of_day a, time_of_day::difference_type b) noexcept {
    return a + -b;
}

inline constexpr time_of_day::difference_type operator-(time_of_day a, time_of_day b) noexcept {
    return a.elapsed_ - b.elapsed_;
}

inline constexpr time_of_day& time_of_day::operator+=(difference_type offset) noexcept {
    return *this = *this + offset;
}

inline constexpr time_of_day& time_of_day::operator-=(difference_type offset) noexcept {
    return *this = *this - offset;
}

inline constexpr bool operator==(time_of_day a, time_of_day b) noexcept {
    return a.elapsed_ == b.elapsed_;
}

inline constexpr bool operator!=(time_of_day a, time_of_day b) noexcept {
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
    constexpr std::size_t operator()(takatori::datetime::time_of_day object) const noexcept {
        return object.time_since_epoch().count();
    }
};
