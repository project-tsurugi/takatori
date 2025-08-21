#pragma once

#include <ostream>
#include <vector>

#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"
#include "details/table_privilege_element.h"

namespace takatori::statement {

/**
 * @brief revoke privileges from tables statement.
 */
class revoke_table final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr statement_kind tag { statement_kind::revoke_table };

    using element_type = details::table_privilege_element;

    /**
     * @brief creates a new instance.
     * @param elements the statement operations for individual tables
     */
    explicit revoke_table(std::vector<element_type> elements) noexcept;

    /**
     * @brief creates a new instance.
     * @param elements the statement operations for individual tables
     */
    revoke_table(std::initializer_list<element_type> elements);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit revoke_table(util::clone_tag_t, revoke_table const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit revoke_table(util::clone_tag_t, revoke_table&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] revoke_table* clone() const& override;
    [[nodiscard]] revoke_table* clone() && override;

    /**
     * @brief returns the statement operations for individual tables.
     * @return the operations for individual tables
     */
    [[nodiscard]] std::vector<element_type>& elements() noexcept;

    /// @copydoc elements()
    [[nodiscard]] std::vector<element_type> const& elements() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(revoke_table const& a, revoke_table const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(revoke_table const& a, revoke_table const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, revoke_table const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::vector<element_type> elements_;
};

} // namespace takatori::statement