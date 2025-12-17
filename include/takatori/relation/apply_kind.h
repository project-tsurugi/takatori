#pragma once

#include <ostream>
#include <string_view>

namespace takatori::relation {

/**
 * @brief represents a apply operator kind.
 */
enum class apply_kind {
    /**
     * @brief represents a CROSS APPLY (a.k.a. `INNER LATERAL JOIN`).
     */
    cross,

    /**
     * @brief represents an OUTER APPLY (a.k.a. `LEFT LATERAL JOIN`).
     */
    outer,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr std::string_view to_string_view(apply_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = apply_kind;
    switch (value) {
        case kind::cross: return "cross"sv;
        case kind::outer: return "outer"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, apply_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::relation
