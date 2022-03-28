#pragma once

#include <optional>
#include <ostream>
#include <vector>

#include <takatori/descriptor/schema.h>
#include <takatori/descriptor/storage.h>
#include <takatori/descriptor/relation.h>
#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"

namespace takatori::statement {

/**
 * @brief table definition statement.
 */
class create_table final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr inline statement_kind tag { statement_kind::create_table };

    /**
     * @brief creates a new instance.
     * @param schema definition target schema
     * @param definition compile-time table information
     * @param primary_key compile-time primary key constraint
     * @param unique_keys compile-time unique key constraints
     */
    explicit create_table(
            descriptor::schema schema,
            descriptor::storage definition,
            descriptor::relation primary_key,
            std::vector<descriptor::relation> unique_keys = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit create_table(util::clone_tag_t, create_table const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit create_table(util::clone_tag_t, create_table&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] create_table* clone() const& override;
    [[nodiscard]] create_table* clone() && override;

    /**
     * @brief returns the target schema of this table definition.
     * @return the target schema
     */
    [[nodiscard]] descriptor::schema& schema() noexcept;

    /// @copydoc schema()
    [[nodiscard]] descriptor::schema const& schema() const noexcept;

    /**
     * @brief returns the compile-time table information.
     * @return the compile-time table information
     */
    [[nodiscard]] descriptor::storage& definition() noexcept;

    /// @copydoc definition()
    [[nodiscard]] descriptor::storage const& definition() const noexcept;

    /**
     * @brief returns the primary key constraint.
     * @details
     *  The returned constraint will refer compile-time table information
     *  returned in definition().
     *  This may return as same as definition().
     * @return the primary key constraint
     * @note if the table does not have any explicit primary key, this will return empty constraint.
     */
    [[nodiscard]] descriptor::relation& primary_key() noexcept;

    /// @copydoc primary_key()
    [[nodiscard]] descriptor::relation const& primary_key() const noexcept;

    /**
     * @brief returns the unique key constraints.
     * @details The returned constraint will refer compile-time table information
     *      returned in definition().
     * @return the list of unique key constraints
     */
    [[nodiscard]] std::vector<descriptor::relation>& unique_keys() noexcept;

    /// @copydoc unique_keys()
    [[nodiscard]] std::vector<descriptor::relation> const& unique_keys() const noexcept;

    // FIXME: check constraints

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(create_table const& a, create_table const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(create_table const& a, create_table const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, create_table const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::schema schema_;
    descriptor::storage definition_;
    descriptor::relation primary_key_;
    std::vector<descriptor::relation> unique_keys_ {};
};

} // namespace takatori::statement