#pragma once

#include <ostream>
#include <string>
#include <string_view>

namespace takatori::relation {

/**
 * @brief represents a comparison semantics kind.
 */
enum class comparison_semantics_kind {

    /**
     * @brief compare two values as SQL standard.
     * @details If either of the two values is NULL, the result is NULL (unknown).
     */
    ternary,

    /**
     * @brief compare two values as NULL-safe.
     * @details Treats NULL as a regular value. NULL compared with NULL evaluates to true,
     * and NULL compared with any non-NULL value evaluates to false.
     */
    binary,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr std::string_view to_string_view(comparison_semantics_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = comparison_semantics_kind;
    switch (value) {
        case kind::ternary: return "ternary"sv;
        case kind::binary: return "binary"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, comparison_semantics_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::relation
