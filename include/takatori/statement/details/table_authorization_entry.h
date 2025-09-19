#pragma once

#include <ostream>
#include <string>
#include <vector>

#include <takatori/util/clone_tag.h>

#include "table_privilege_action.h"
#include "../authorization_user_kind.h"

namespace takatori::statement::details {

/**
 * @brief represents an authorization for the target about table privileges.
 */
class table_authorization_entry {
public:
    /// @brief the user kind type.
    using user_kind_type = authorization_user_kind;

    /// @brief the identifier type.
    using identifier_type = std::string;

    /**
     * @brief creates a new instance.
     * @param user_kind the kind of target
     * @param authorization_identifier the identifier associated with this entry
     *      only if user_kind is authorization_user_kind::specified,
     *      otherwise empty string
     * @param privileges the list of privileges
     */
    table_authorization_entry(
            user_kind_type user_kind,
            identifier_type authorization_identifier,
            std::vector<table_privilege_action> privileges) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit table_authorization_entry(util::clone_tag_t, table_authorization_entry const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit table_authorization_entry(util::clone_tag_t, table_authorization_entry&& other);

    /**
     * @brief returns the authorization target kind.
     * @return return target kind
     */
    [[nodiscard]] user_kind_type& user_kind() noexcept;

    /// @copydoc user_kind()
    [[nodiscard]] user_kind_type const& user_kind() const noexcept;

    /**
     * @brief returns the identifier associated with this entry.
     * @details This may be a username, role name, or any other identifier
     * @return the authorization identifier if user_kind is authorization_user_kind::specified
     * @return empty if user_kind is not authorization_user_kind::specified
     */
    [[nodiscard]] identifier_type& authorization_identifier() noexcept;

    /// @copydoc authorization_identifier()
    [[nodiscard]] identifier_type const& authorization_identifier() const noexcept;

    /**
     * @brief returns the list of privileges.
     * @return the list of privileges
     */
    [[nodiscard]] std::vector<table_privilege_action>& privileges() noexcept;

    /// @copydoc privileges()
    [[nodiscard]] std::vector<table_privilege_action> const& privileges() const noexcept;

private:
    user_kind_type user_kind_;
    identifier_type authorization_identifier_;
    std::vector<table_privilege_action> privileges_;
};

/**
 * @brief returns whether the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(table_authorization_entry const& a, table_authorization_entry const& b) noexcept;

/**
 * @brief returns whether the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(table_authorization_entry const& a, table_authorization_entry const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, table_authorization_entry const& value);


} // namespace takatori::statement::details
