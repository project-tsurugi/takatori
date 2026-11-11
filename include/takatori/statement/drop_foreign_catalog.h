#pragma once

#include <ostream>
#include <string>

#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"
#include "drop_foreign_catalog_option_kind.h"

namespace takatori::statement {

/**
 * @brief represents a statement to remove a foreign (remote) catalog.
 * @details This statement holds the catalog name after the identifier normalization of the SQL analyzer,
 *      and does not resolve it; the SQL execution engine resolves it and reports the related errors.
 */
class drop_foreign_catalog final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr statement_kind tag { statement_kind::drop_foreign_catalog };

    /// @brief the type of catalog name.
    using name_type = std::string;

    /// @brief set of options for this statement.
    using option_set_type = drop_foreign_catalog_option_kind_set;

    /**
     * @brief creates a new instance.
     * @param catalog_name the name of the catalog to be removed
     * @param options the options for this statement
     */
    explicit drop_foreign_catalog(
            name_type catalog_name,
            option_set_type options = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit drop_foreign_catalog(util::clone_tag_t, drop_foreign_catalog const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit drop_foreign_catalog(util::clone_tag_t, drop_foreign_catalog&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] drop_foreign_catalog* clone() const& override;
    [[nodiscard]] drop_foreign_catalog* clone() && override;

    /**
     * @brief returns the name of the catalog to be removed.
     * @return the catalog name
     */
    [[nodiscard]] name_type& catalog_name() noexcept;

    /// @copydoc catalog_name()
    [[nodiscard]] name_type const& catalog_name() const noexcept;

    /**
     * @brief returns the options for this statement.
     * @return the options
     */
    [[nodiscard]] option_set_type& options() noexcept;

    /// @copydoc options()
    [[nodiscard]] option_set_type const& options() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(drop_foreign_catalog const& a, drop_foreign_catalog const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(drop_foreign_catalog const& a, drop_foreign_catalog const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, drop_foreign_catalog const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    name_type catalog_name_;
    option_set_type options_;
};

} // namespace takatori::statement
