#pragma once

#include <ostream>
#include <string>

#include <takatori/descriptor/storage.h>
#include <takatori/descriptor/variable.h>
#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"

namespace takatori::statement {

/**
 * @brief rename table column statement.
 */
class rename_column final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr statement_kind tag { statement_kind::rename_column };

    /// @brief the name type.
    using name_type = std::string;

    /**
     * @brief creates a new instance.
     * @param table where the target column is declared
     * @param target target column
     * @param replacement the replacement name
     */
    explicit rename_column(
            descriptor::storage table,
            descriptor::variable target,
            name_type replacement) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit rename_column(util::clone_tag_t, rename_column const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit rename_column(util::clone_tag_t, rename_column&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] rename_column* clone() const& override;
    [[nodiscard]] rename_column* clone() && override;

    /**
     * @brief returns the schema of target table.
     * @return the target schema
     */
    [[nodiscard]] descriptor::storage& table() noexcept;

    /// @copydoc table()
    [[nodiscard]] descriptor::storage const& table() const noexcept;

    /**
     * @brief returns the target column.
     * @return the target column
     */
    [[nodiscard]] descriptor::variable& target() noexcept;

    /// @copydoc target()
    [[nodiscard]] descriptor::variable const& target() const noexcept;

    /**
     * @brief returns the replacement name.
     * @return the replacement name
     */
    [[nodiscard]] name_type& replacement() noexcept;

    /// @copydoc replacement()
    [[nodiscard]] name_type const& replacement() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(rename_column const& a, rename_column const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(rename_column const& a, rename_column const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, rename_column const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::storage table_;
    descriptor::variable target_;
    name_type replacement_;
};

} // namespace takatori::statement