#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include <takatori/util/enum_set.h>

namespace takatori::relation {

/**
 * @brief represents properties about the row order of emit operations.
 * @details Each property is effective only if the input relation is an ordered relation.
 */
enum class emit_ordering : std::size_t {
    /**
     * @brief emits the rows in the order defined by the input relation.
     * @details If this is not specified, or the input relation is not an ordered relation,
     *      the order of the emitted rows is not specified.
     */
    preserve_input = 0,
};

/// @brief set of emit_ordering.
using emit_ordering_set = util::enum_set<
        emit_ordering,
        emit_ordering::preserve_input,
        emit_ordering::preserve_input>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(emit_ordering value) noexcept {
    using namespace std::string_view_literals;
    using kind = emit_ordering;
    switch (value) {
        case kind::preserve_input: return "preserve_input"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, emit_ordering value) {
    return out << to_string_view(value);
}

} // namespace takatori::relation
