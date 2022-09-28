#pragma once

#include <ostream>

namespace takatori::type {

/**
 * @brief represents the data type has "varying" attribute.
 */
class varying_t {
public:
    /**
     * @brief creates a new instance.
     */
    constexpr explicit varying_t(bool enabled) noexcept : enabled_(enabled) {}

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
    constexpr varying_t operator~() const noexcept {
        return varying_t { !enabled() };
    }

    /// @copydoc enabled()
    constexpr explicit operator bool() const noexcept {
        return enabled();
    }

private:
    bool enabled_ { true };
};

/// @copydoc varying_t
constexpr inline varying_t varying { true };

/**
 * @brief appends string representation of varying_t.
 * @param out the target output
 * @param value the value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, varying_t value) {
    if (value) {
        return out << "varying";
    }
    return out << "not varying";
}

} // namespace takatori::type
