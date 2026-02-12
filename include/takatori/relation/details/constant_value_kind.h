#pragma once

#include <cstddef>
#include <ostream>
#include <string_view>

namespace takatori::relation::details {

/**
 * @brief represents representation kind of constant values.
 */
enum class constant_value_kind : std::size_t {

    /// @brief the constant value is absent.
    absent = 0,

    /// @brief the constant value is represented as an immediate value.
    immediate = 1,

    /// @brief the constant value is represented as a variable (ordinally it is a host variable).
    variable = 2,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr std::string_view to_string_view(constant_value_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = constant_value_kind;
    switch (value) {
        case kind::absent: return "absent"sv;
        case kind::immediate: return "immediate"sv;
        case kind::variable: return "variable"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, constant_value_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::relation::details
