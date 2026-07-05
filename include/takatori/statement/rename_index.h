#pragma once

#include <ostream>
#include <string>

#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/schema.h>
#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"

namespace takatori::statement {

/**
 * @brief rename index statement.
 */
class rename_index final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr statement_kind tag { statement_kind::rename_index };

    /// @brief the name type.
    using name_type = std::string;

    /**
     * @brief creates a new instance.
     * @param schema where the target index is declared
     * @param target target index
     * @param replacement the replacement name
     */
    explicit rename_index(
            descriptor::schema schema,
            descriptor::relation target,
            name_type replacement) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit rename_index(util::clone_tag_t, rename_index const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit rename_index(util::clone_tag_t, rename_index&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] rename_index* clone() const& override;
    [[nodiscard]] rename_index* clone() && override;

    /**
     * @brief returns the schema of target index.
     * @return the target schema
     */
    [[nodiscard]] descriptor::schema& schema() noexcept;

    /// @copydoc schema()
    [[nodiscard]] descriptor::schema const& schema() const noexcept;

    /**
     * @brief returns the target index.
     * @return the target index
     */
    [[nodiscard]] descriptor::relation& target() noexcept;

    /// @copydoc target()
    [[nodiscard]] descriptor::relation const& target() const noexcept;

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
    friend bool operator==(rename_index const& a, rename_index const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(rename_index const& a, rename_index const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, rename_index const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::schema schema_;
    descriptor::relation target_;
    name_type replacement_;
};

} // namespace takatori::statement