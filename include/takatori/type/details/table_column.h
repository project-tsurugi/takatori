#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "../data.h"

namespace takatori::type::details {

/**
 * @brief represents a column of a table type.
 */
class table_column {
public:
    /// @brief the name type.
    using name_type = std::string;

    /**
     * @brief creates a new instance.
     * @param name the column name
     * @param type the column type
     */
    table_column(name_type name, std::shared_ptr<data const> type) noexcept;

    /**
     * @brief creates a new instance.
     * @param name the column name
     * @param type the column type
     */
    table_column(std::string_view name, data&& type);

    /**
     * @brief returns the column name.
     * @return the column name
     */
    [[nodiscard]] std::string_view name() const noexcept;

    /**
     * @brief returns the column type.
     * @return the column type
     */
    [[nodiscard]] data const& type() const noexcept;

    /**
     * @brief returns the column type as shared pointer.
     * @return the column type
     */
    [[nodiscard]] std::shared_ptr<data const> shared_type() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(table_column const& a, table_column const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(table_column const& a, table_column const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, table_column const& value);

private:
    name_type name_;
    std::shared_ptr<data const> type_;
};

} // namespace takatori::type:details
