#pragma once

#include <ostream>
#include <utility>

namespace takatori::datetime::parser {

struct region {
    /// @brief the position type.
    using position_type = std::size_t;

    /// @brief represents an invalid position.
    static constexpr position_type npos = static_cast<position_type>(-1);

    /// @brief the beginning position (inclusive, 0-origin).
    position_type begin { npos }; // NOLINT(misc-non-private-member-variables-in-classes): for parser generator's convention

    /// @brief the ending position (exclusive, 0-origin).
    position_type end { npos }; // NOLINT(misc-non-private-member-variables-in-classes): for parser generator's convention

    /**
     * @brief creates a new instance with empty region.
     */
    constexpr region() = default;

    /**
     * @brief creates a new instance.
     * @param begin the beginning position (inclusive, 0-origin)
     * @param end the ending position (exclusive, 0-origin)
     */
    constexpr region(position_type begin, position_type end) noexcept :
        begin { begin },
        end { end }
    {}

    /**
     * @brief returns whether or not this region is valid.
     * @return true if the region is valid
     * @return false if begin or end is not valid
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return begin != npos && end != npos;
    }

    /**
     * @brief returns the beginning position of this region.
     * @return the beginning position (inclusive)
     */
    [[nodiscard]] constexpr position_type first() const noexcept {
        return begin;
    }

    /**
     * @brief returns the ending position of this region.
     * @return the ending position (exclusive)
     */
    [[nodiscard]] constexpr position_type last() const noexcept {
        return end;
    }

    /**
     * @brief returns the size of this region.
     * @return the number of position in this region
     */
    [[nodiscard]] constexpr position_type size() const noexcept {
        return end - begin;
    }
};

/**
 * @brief returns the union of the two regions.
 * @details The regions need not have intersections.
 * @param a the first region
 * @param b the second region
 * @return the union of the regions
 */
[[nodiscard]] region operator|(region a, region b) noexcept;

/**
 * @brief returns whether or not the two regions are equivalent.
 * @param a the first region
 * @param b the second region
 * @return true if the both are equivalent
 * @return false otherwise
 */
[[nodiscard]] bool operator==(region a, region b) noexcept;

/**
 * @brief returns whether or not the two regions are different.
 * @param a the first region
 * @param b the second region
 * @return true if the both are different
 * @return false otherwise
 */
[[nodiscard]] bool operator!=(region a, region b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, region value);

} // namespace takatori::datetime::parser
