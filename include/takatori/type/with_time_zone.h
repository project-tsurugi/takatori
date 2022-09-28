#pragma once

#include <ostream>

namespace takatori::type {

/**
 * @brief represents the data type has "with_time_zone" attribute.
 */
class with_time_zone_t {
public:
    /**
     * @brief creates a new instance.
     */
    constexpr explicit with_time_zone_t(bool enabled) noexcept : enabled_ { enabled } {}

    /**
     * @brief returns whether or not this attribute is enabled.
     * @return true if this attribute is enabled
     * @return false otherwise
     */
    [[nodiscard]] constexpr bool enabled() const noexcept {
        return enabled_;
    }

    /**
     * @brief flips enable/disable this attribute.
     * @return the flipped attribute
     */
    constexpr with_time_zone_t operator~() const noexcept {
        return with_time_zone_t { !enabled() };
    }

    /// @copydoc enabled()
    constexpr explicit operator bool() const noexcept {
        return enabled();
    }

private:
    bool enabled_ { true };
};

/// @copydoc with_time_zone_t
constexpr inline with_time_zone_t with_time_zone { true };

/**
 * @brief appends string representation of with_time_zone_t.
 * @param out the target output
 * @param value the value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, with_time_zone_t value) {
    if (value) {
        return out << "with_time_zone";
    }
    return out << "without_time_zone";
}

} // namespace takatori::type
