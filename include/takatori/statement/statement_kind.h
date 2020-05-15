#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#include <cstdlib>

#include <takatori/util/enum_set.h>

namespace takatori::statement {

/**
 * @brief represents kind of procedural statements.
 */
enum class statement_kind {

    // DML
    /// @brief execute statement to perform its own step execution plan.
    execute,

    /// @brief emit rows onto table.
    write,

    // special statements

    /// @brief custom statement for compiler or third party extension.
    extension,
};

/// @brief a set of statement_kind.
using statement_kind_set = util::enum_set<
        statement_kind,
        statement_kind::execute,
        statement_kind::extension>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(statement_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = statement_kind;
    switch (value) {
        case kind::execute: return "execute"sv;
        case kind::write: return "write"sv;
        case kind::extension: return "extension"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, statement_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::statement
