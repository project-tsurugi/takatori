#pragma once

#include <chrono>
#include <functional>
#include <iostream>

#include <cstdint>

namespace takatori::chrono {

class time_point;

/**
 * @brief represents time of day.
 */
class time_of_day {
public:
    /**
     * @brief creates a new instance which represents 00:00:00 of day.
     */
    constexpr time_of_day() = default;

    /**
     * @brief creates a new instance.
     * @param nanoseconds the elapsed time since 00:00:00 of day, in nano-seconds
     */
    explicit constexpr time_of_day(std::uint64_t nanoseconds) noexcept;

    /**
     * @brief creates a new instance.
     * @tparam Rep the representation type
     * @tparam Period the tick period
     * @param elapsed the elapsed time since 00:00:00 of day
     */
    template<class Rep, class Period>
    constexpr time_of_day(std::chrono::duration<Rep, Period> elapsed) noexcept; // NOLINT

    /**
     * @brief returns the elapsed time since 00:00:00 of day in nano-second.
     * @return the the elapsed time since 00:00:00
     */
    constexpr std::uint64_t total_nanoseconds() const noexcept;

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
    std::uint64_t total_nanoseconds_ {};

    static constexpr std::uint64_t normalize(std::int64_t time) noexcept {
        constexpr std::int64_t day = 86'400'000'000'000LL;
        if (time < 0) {
            return static_cast<std::uint64_t>((time % day) + day);
        }
        return static_cast<std::uint64_t>(time % day);
    }

    static constexpr std::uint64_t normalize(std::uint64_t time) noexcept {
        constexpr std::uint64_t day = 86'400'000'000'000ULL;
        return time % day;
    }

    std::ostream& print_body(std::ostream& out) const;

    friend class time_point;
};

constexpr time_of_day::time_of_day(std::uint64_t nanoseconds) noexcept
    : total_nanoseconds_(normalize(nanoseconds))
{}

template<class Rep, class Period>
inline constexpr time_of_day::time_of_day(std::chrono::duration<Rep, Period> elapsed) noexcept
    : total_nanoseconds_(normalize(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count()))
{}

inline constexpr std::uint64_t time_of_day::total_nanoseconds() const noexcept {
    return total_nanoseconds_;
}

inline constexpr bool operator==(time_of_day const& a, time_of_day const& b) noexcept {
    return a.total_nanoseconds_ == b.total_nanoseconds_;
}

inline constexpr bool operator!=(time_of_day const& a, time_of_day const& b) noexcept {
    return !(a == b);
}

} // namespace takatori::chrono

/// @brief provides hash code of takatori::chrono::time_of_day.
template<> struct std::hash<takatori::chrono::time_of_day> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::chrono::time_of_day const& object) const noexcept {
        return static_cast<std::size_t>(object.total_nanoseconds());
    }
};
