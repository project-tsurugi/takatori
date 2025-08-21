#pragma once

#include <ostream>

#include <takatori/util/clone_tag.h>

#include "../table_action_kind.h"

namespace takatori::statement::details {

/**
 * @brief represents an action to be performed on the table.
 * @details this is used to represent a privilege action on the table.
 */
class table_privilege_action {
public:
    /// @brief the action kind.
    using action_kind_type = table_action_kind;

    /**
     * @brief creates a new instance that represents any control of the table.
     */
    table_privilege_action() noexcept;

    /**
     * @brief creates a new instance.
     * @param kind the action kind
     */
    table_privilege_action(action_kind_type kind) noexcept; // NOLINT

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit table_privilege_action(util::clone_tag_t, table_privilege_action const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit table_privilege_action(util::clone_tag_t, table_privilege_action&& other);

    /**
     * @brief returns the kind of this action.
     * @return the action kind
     */
    [[nodiscard]] action_kind_type& action_kind() noexcept;

    /// @copydoc action_kind()
    [[nodiscard]] action_kind_type const& action_kind() const noexcept;

private:
    action_kind_type action_kind_ { action_kind_type::control };
};

/**
 * @brief returns whether the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(table_privilege_action const& a, table_privilege_action const& b) noexcept;

/**
 * @brief returns whether the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(table_privilege_action const& a, table_privilege_action const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, table_privilege_action const& value);


} // namespace takatori::statement::details
