#pragma once

#include <ostream>
#include <string_view>

#include <takatori/util/enum_set.h>

namespace takatori::statement {

/**
 * @brief kind of actions to be performed on the table..
 */
enum class table_action_kind {
    /// @brief any control of the table.
    control,

    /// @brief action to read entries from the table (`SELECT`)
    select,

    /// @brief action to create entries into the table (`INSERT`).
    insert,

    /// @brief action to update entries in the table (`UPDATE`).
    update,

    /// @brief action to delete entries from the table (`DELETE`).
    delete_,
};

/// @brief a set of table_action_kind.
using table_action_kind_set = util::enum_set<
        table_action_kind,
        table_action_kind::insert,
        table_action_kind::delete_>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(table_action_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = table_action_kind;
    switch (value) {
        case kind::control: return "control"sv;
        case kind::select: return "select"sv;
        case kind::insert: return "insert"sv;
        case kind::update: return "update"sv;
        case kind::delete_: return "delete"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, table_action_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::statement
