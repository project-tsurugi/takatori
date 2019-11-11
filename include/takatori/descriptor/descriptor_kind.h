#pragma once

#include <iostream>
#include <string>
#include <string_view>

namespace takatori::descriptor {

/**
 * @brief represents kind of descriptor.
 */
enum class descriptor_kind {

    /// @brief describes a value.
    value,

    /// @brief describes a variable, or named value.
    variable,

    /// @brief describes a function.
    function,

    /// @brief describes a data type.
    data_type,

    /// @brief describes a query.
    query,

    /// @brief describes a time zone.
    time_zone,
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
        case kind::value: return "value"sv;
        case kind::variable: return "variable"sv;
        case kind::function: return "function"sv;
        case kind::data_type: return "data_type"sv;
        case kind::query: return "query"sv;
        case kind::time_zone: return "time_zone"sv;
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
