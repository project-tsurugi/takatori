#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include <takatori/util/enum_set.h>

namespace takatori::statement {

/**
 * @brief represents options for truncate table statement.
 */
enum class truncate_table_option_kind : std::size_t {
    /// @brief restart underlying identity values from their initial values.
    restart_identity = 0,
};

/// @brief set of truncate table options.
using truncate_table_option_kind_set = util::enum_set<
        truncate_table_option_kind,
        truncate_table_option_kind::restart_identity,
        truncate_table_option_kind::restart_identity>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr std::string_view to_string_view(truncate_table_option_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = truncate_table_option_kind;
    switch (value) {
        case kind::restart_identity: return "restart_identity"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, truncate_table_option_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::statement
