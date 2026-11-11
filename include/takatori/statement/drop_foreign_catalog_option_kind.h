#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include <takatori/util/enum_set.h>

namespace takatori::statement {

/**
 * @brief represents options of drop foreign catalog statement.
 */
enum class drop_foreign_catalog_option_kind : std::size_t {
    /// @brief if the catalog does not exist, this statement does nothing and succeeds.
    if_catalog_exists = 0,
};

/// @brief set of drop foreign catalog options.
using drop_foreign_catalog_option_kind_set = util::enum_set<
        drop_foreign_catalog_option_kind,
        drop_foreign_catalog_option_kind::if_catalog_exists,
        drop_foreign_catalog_option_kind::if_catalog_exists>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr std::string_view to_string_view(drop_foreign_catalog_option_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = drop_foreign_catalog_option_kind;
    switch (value) {
        case kind::if_catalog_exists: return "if_catalog_exists"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, drop_foreign_catalog_option_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::statement
