#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include <takatori/util/enum_set.h>

namespace takatori::plan {

/**
 * @brief represents kind of step in execution plan.
 */
enum class step_kind {
    /// @brief process.
    process,
    /// @brief forward exchange.
    forward,
    /// @brief shuffle exchange for building each group.
    group,
    /// @brief shuffle exchange for aggregating each group.
    aggregate,
    /// @brief broadcast exchange.
    broadcast,
    /// @brief discard exchange.
    discard,

    /// FIXME: transitive closure
};

/// @brief a set of step_kind.
using step_kind_set = util::enum_set<
        step_kind,
        step_kind::process,
        step_kind::discard>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(step_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = step_kind;
    switch (value) {
        case kind::process: return "process"sv;
        case kind::forward: return "forward"sv;
        case kind::group: return "group"sv;
        case kind::aggregate: return "aggregate"sv;
        case kind::broadcast: return "broadcast"sv;
        case kind::discard: return "discard"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, step_kind value) {
    return out << to_string_view(value);
}

/**
 * @brief represents category of step in execution plan.
 */
enum class step_category {
    /// @brief process category.
    process,
    /// @brief exchange category.
    exchange,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(step_category value) noexcept {
    using namespace std::string_view_literals;
    using kind = step_category;
    switch (value) {
        case kind::process: return "process"sv;
        case kind::exchange: return "exchange"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, step_category value) {
    return out << to_string_view(value);
}

/**
 * @brief returns the category of the given step kind.
 * @param kind the step kind
 * @return the corresponded category
 */
inline constexpr step_category category_of(step_kind kind) {
    switch (kind) {
        case step_kind::process:
            return step_category::process;

        case step_kind::forward:
        case step_kind::group:
        case step_kind::aggregate:
        case step_kind::broadcast:
        case step_kind::discard:
            return step_category::exchange;
    }
    std::abort();
}

} // namespace takatori::plan
