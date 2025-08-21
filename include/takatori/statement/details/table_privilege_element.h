#pragma once

#include <ostream>
#include <vector>

#include <takatori/descriptor/storage.h>

#include <takatori/util/clone_tag.h>

#include "table_authorization_entry.h"
#include "table_privilege_action.h"

namespace takatori::statement::details {

/**
 * @brief an element about manipulating table privileges.
 */
class table_privilege_element {
public:

    /**
     * @brief creates a new instance.
     * @param table the target table descriptor
     * @param default_privileges the list of default privileges (for all users)
     * @param authorization_entries the list of authorizations (for individual users or roles)
     */
    table_privilege_element(
            descriptor::storage table,
            std::vector<table_privilege_action> default_privileges,
            std::vector<table_authorization_entry> authorization_entries) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit table_privilege_element(util::clone_tag_t, table_privilege_element const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit table_privilege_element(util::clone_tag_t, table_privilege_element&& other);

    /**
     * @brief returns the target table descriptor.
     * @return the table descriptor
     */
    [[nodiscard]] descriptor::storage& table() noexcept;

    /// @copydoc table()
    [[nodiscard]] descriptor::storage const& table() const noexcept;

    /**
     * @brief returns the list of privileges.
     * @return the list of privileges
     */
    [[nodiscard]] std::vector<table_privilege_action>& default_privileges() noexcept;

    /// @copydoc default_privileges()
    [[nodiscard]] std::vector<table_privilege_action> const& default_privileges() const noexcept;

    /**
     * @brief returns the list of authorizations.
     * @return the list of authorizations
     */
    [[nodiscard]] std::vector<table_authorization_entry>& authorization_entries() noexcept;

    /// @copydoc authorization_entries()
    [[nodiscard]] std::vector<table_authorization_entry> const& authorization_entries() const noexcept;

private:
    descriptor::storage table_;
    std::vector<table_privilege_action> default_privileges_;
    std::vector<table_authorization_entry> authorization_entries_;
};

/**
 * @brief returns whether the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(table_privilege_element const& a, table_privilege_element const& b) noexcept;

/**
 * @brief returns whether the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(table_privilege_element const& a, table_privilege_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, table_privilege_element const& value);

} // namespace takatori::statement::details
