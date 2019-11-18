#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace takatori::graph {

/**
 * @brief represents the port direction.
 */
enum class port_direction {

    /// @brief input port.
    input,

    /// @brief output port.
    output,
};

/**
 * @brief returns the opposite direction.
 * @param value the direction
 * @return the opposite direction
 */
inline constexpr port_direction operator~(port_direction value) noexcept {
    using kind = port_direction;
    switch (value) {
        case kind::input: return kind::output;
        case kind::output: return kind::input;
    }
    std::abort();
}

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(port_direction value) noexcept {
    using namespace std::string_view_literals;
    using kind = port_direction;
    switch (value) {
        case kind::input: return "input"sv;
        case kind::output: return "output"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, port_direction value) {
    return out << to_string_view(value);
}

} // namespace takatori::graph
