#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include <cstdlib>

namespace takatori::relation {

/**
 * @brief represents the direction to read an index.
 * @details The direction is relative to the storage order of the index entries,
 *      and it does not relate to the ascendant or descendant order of the individual index keys.
 */
enum class scan_direction {
    /// @brief reads the index entries in the storage order.
    forward,
    /// @brief reads the index entries in the reverse of the storage order.
    backward,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(scan_direction value) noexcept {
    using namespace std::string_view_literals;
    using kind = scan_direction;
    switch (value) {
        case kind::forward: return "forward"sv;
        case kind::backward: return "backward"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, scan_direction value) {
    return out << to_string_view(value);
}

} // namespace takatori::relation
