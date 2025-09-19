#pragma once

#include <ostream>
#include <string>
#include <string_view>

namespace takatori::statement {

/**
 * @brief represents kind of authorization user.
 */
enum class authorization_user_kind {

    /**
     * @brief the specified user or role.
     */
    specified,

    /**
     * @brief all users except the current user.
     */
    all_users,

    /**
     * @brief the current user.
     */
    current_user,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(authorization_user_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = authorization_user_kind;
    switch (value) {
        case kind::specified: return "specified"sv;
        case kind::all_users: return "all_users"sv;
        case kind::current_user: return "current_user"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, authorization_user_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::statement
