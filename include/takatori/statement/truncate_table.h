#pragma once

#include <ostream>

#include <takatori/descriptor/storage.h>
#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"
#include "truncate_table_option_kind.h"

namespace takatori::statement {

/**
 * @brief drop table statement.
 */
class truncate_table final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr statement_kind tag { statement_kind::truncate_table };

    /// @brief set of options for this statement.
    using option_set_type = truncate_table_option_kind_set;

    /**
     * @brief creates a new instance.
     * @param target truncate target table
     * @param options the options for this statement
     */
    explicit truncate_table(
            descriptor::storage target,
            option_set_type options = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit truncate_table(util::clone_tag_t, truncate_table const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit truncate_table(util::clone_tag_t, truncate_table&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] truncate_table* clone() const& override;
    [[nodiscard]] truncate_table* clone() && override;

    /**
     * @brief returns the target table.
     * @return the target table
     */
    [[nodiscard]] descriptor::storage& target() noexcept;

    /// @copydoc target()
    [[nodiscard]] descriptor::storage const& target() const noexcept;

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
    friend bool operator==(truncate_table const& a, truncate_table const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(truncate_table const& a, truncate_table const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, truncate_table const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::storage target_;
    option_set_type options_;
};

} // namespace takatori::statement