#pragma once

#include <iostream>

namespace takatori::type {

/**
 * @brief represents the data type has "varying" attribute.
 */
struct varying_t {
    /**
     * @brief creates a new instance.
     */
    explicit varying_t() = default;
};

/// @copydoc varying_t
constexpr inline varying_t varying;

/**
 * @brief appends string representation of varying_t.
 * @param out the target output
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, varying_t) {
    return out << "varying";
}

} // namespace takatori::type
