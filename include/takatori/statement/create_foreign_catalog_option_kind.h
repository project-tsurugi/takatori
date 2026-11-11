#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include <takatori/util/enum_set.h>

namespace takatori::statement {

/**
 * @brief represents options of create foreign catalog statement.
 */
enum class create_foreign_catalog_option_kind : std::size_t {
    /// @brief if the catalog already exists, this statement does nothing and succeeds.
    if_catalog_not_exists = 0,

    /// @brief if the server does not exist, this statement does nothing and succeeds.
    if_server_exists,
};

/// @brief set of create foreign catalog options.
using create_foreign_catalog_option_kind_set = util::enum_set<
        create_foreign_catalog_option_kind,
        create_foreign_catalog_option_kind::if_catalog_not_exists,
        create_foreign_catalog_option_kind::if_server_exists>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr std::string_view to_string_view(create_foreign_catalog_option_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = create_foreign_catalog_option_kind;
    switch (value) {
        case kind::if_catalog_not_exists: return "if_catalog_not_exists"sv;
        case kind::if_server_exists: return "if_server_exists"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, create_foreign_catalog_option_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::statement
