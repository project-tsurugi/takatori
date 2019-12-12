#pragma once

#include <iostream>
#include <string>
#include <string_view>

namespace takatori::descriptor {

/**
 * @brief represents kind of descriptor.
 */
enum class descriptor_kind {

    /// @brief describes a variable, or named value.
    variable,

    /// @brief describes an external relation (index, broadcast data, or etc.).
    relation,

    /// @brief describes a function.
    function,

    /// @brief describes an aggregate function.
    aggregate_function,

    /// @brief describes a declared type.
    declared_type,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(descriptor_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = descriptor_kind;
    switch (value) {
        case kind::variable: return "variable"sv;
        case kind::relation: return "relation"sv;
        case kind::function: return "function"sv;
        case kind::aggregate_function: return "aggregate_function"sv;
        case kind::declared_type: return "declared_type"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, descriptor_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::descriptor
