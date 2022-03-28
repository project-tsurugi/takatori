#pragma once

#include <ostream>

#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/schema.h>
#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"

namespace takatori::statement {

/**
 * @brief drop index statement.
 */
class drop_index final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr inline statement_kind tag { statement_kind::drop_index };

    /**
     * @brief creates a new instance.
     * @param schema drop target schema
     * @param target drop target index
     */
    explicit drop_index(
            descriptor::schema schema,
            descriptor::relation target) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit drop_index(util::clone_tag_t, drop_index const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit drop_index(util::clone_tag_t, drop_index&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] drop_index* clone() const& override;
    [[nodiscard]] drop_index* clone() && override;

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
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(drop_index const& a, drop_index const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(drop_index const& a, drop_index const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, drop_index const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::schema schema_;
    descriptor::relation target_;
};

} // namespace takatori::statement