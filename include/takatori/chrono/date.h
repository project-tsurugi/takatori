#pragma once

#include <functional>
#include <iostream>

#include <cstdint>

namespace takatori::chrono {

class time_point;

/**
 * @brief represents date since 1900-01-01 GMT.
 */
class date {
public:
    /**
     * @brief creates a new instance which represents 1900-01-01 GMT.
     */
    constexpr date() = default;

    /**
     * @brief creates a new instance.
     * @param days the elapsed days since 1900-01-01 GMT
     */
    explicit constexpr date(std::uint64_t days) noexcept;

    /**
     * @brief returns the elapsed days since 1900-01-01 GMT.
     * @return the the elapsed days
     */
    constexpr std::uint64_t total_days() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(date const& a, date const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(date const& a, date const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, date const& value);

private:
    std::uint64_t total_days_ {};

    std::ostream& print_body(std::ostream& out) const;

    friend class time_point;
};

constexpr date::date(std::uint64_t days) noexcept
    : total_days_(days)
{}

inline constexpr std::uint64_t date::total_days() const noexcept {
    return total_days_;
}

inline constexpr bool operator==(date const& a, date const& b) noexcept {
    return a.total_days_ == b.total_days_;
}

inline constexpr bool operator!=(date const& a, date const& b) noexcept {
    return !(a == b);
}

} // namespace takatori::chrono

/// @brief provides hash code of takatori::chrono::date.
template<> struct std::hash<takatori::chrono::date> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    constexpr std::size_t operator()(takatori::chrono::date const& object) const noexcept {
        return static_cast<std::size_t>(object.total_days());
    }
};
