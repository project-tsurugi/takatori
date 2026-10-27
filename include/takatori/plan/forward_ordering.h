#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include <takatori/util/enum_set.h>

namespace takatori::plan {

/**
 * @brief represents properties about the row order of forward exchanges.
 * @details Each property is effective only if the input relation is an ordered relation.
 */
enum class forward_ordering : std::size_t {
    /**
     * @brief takes the row slice in the order defined by the input relation.
     * @details If this is not specified, or the input relation is not an ordered relation,
     *      the row order to take the row slice is not specified.
     */
    respect_input = 0,

    /**
     * @brief provides an ordered relation in the order defined by the input relation.
     * @details If the row slice is specified, the rows in the slice are also provided in that order.
     *      If this is not specified, or the input relation is not an ordered relation,
     *      the output relation is not an ordered relation.
     */
    preserve_output = 1,
};

/// @brief set of forward_ordering.
using forward_ordering_set = util::enum_set<
        forward_ordering,
        forward_ordering::respect_input,
        forward_ordering::preserve_output>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(forward_ordering value) noexcept {
    using namespace std::string_view_literals;
    using kind = forward_ordering;
    switch (value) {
        case kind::respect_input: return "respect_input"sv;
        case kind::preserve_output: return "preserve_output"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, forward_ordering value) {
    return out << to_string_view(value);
}

} // namespace takatori::plan
