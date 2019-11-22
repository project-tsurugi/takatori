#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace takatori::relation {

/**
 * @brief represents an end-point kind of intervals.
 */
enum class endpoint_kind {

    /// @brief end-point is unspecified (unbound interval).
    unbound,

    /// @brief includes end-point key.
    inclusive,

    /// @brief excludes end-point key.
    exclusive,

    /// @brief includes entries which contain the end-point key as prefix.
    prefixed_inclusive,

    /// @brief excludes entries which contain the end-point key as prefix.
    prefixed_exclusive,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(endpoint_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = endpoint_kind;
    switch (value) {
        case kind::unbound: return "unbound"sv;
        case kind::inclusive: return "inclusive"sv;
        case kind::exclusive: return "exclusive"sv;
        case kind::prefixed_inclusive: return "prefixed_inclusive"sv;
        case kind::prefixed_exclusive: return "prefixed_exclusive"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, endpoint_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::relation
