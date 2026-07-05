#pragma once

#include <ostream>
#include <string>

#include <takatori/descriptor/storage.h>
#include <takatori/descriptor/schema.h>
#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"

namespace takatori::statement {

/**
 * @brief rename table statement.
 */
class rename_table final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr statement_kind tag { statement_kind::rename_table };

    /// @brief the name type.
    using name_type = std::string;

    /**
     * @brief creates a new instance.
     * @param schema where the target table is declared
     * @param target target table
     * @param replacement the replacement name
     */
    explicit rename_table(
            descriptor::schema schema,
            descriptor::storage target,
            name_type replacement) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit rename_table(util::clone_tag_t, rename_table const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit rename_table(util::clone_tag_t, rename_table&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] rename_table* clone() const& override;
    [[nodiscard]] rename_table* clone() && override;

    /**
     * @brief returns the schema of target table.
     * @return the target schema
     */
    [[nodiscard]] descriptor::schema& schema() noexcept;

    /// @copydoc schema()
    [[nodiscard]] descriptor::schema const& schema() const noexcept;

    /**
     * @brief returns the target table.
     * @return the target table
     */
    [[nodiscard]] descriptor::storage& target() noexcept;

    /// @copydoc target()
    [[nodiscard]] descriptor::storage const& target() const noexcept;

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
    friend bool operator==(rename_table const& a, rename_table const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(rename_table const& a, rename_table const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, rename_table const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::schema schema_;
    descriptor::storage target_;
    name_type replacement_;
};

} // namespace takatori::statement