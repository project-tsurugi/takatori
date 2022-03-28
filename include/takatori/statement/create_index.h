#pragma once

#include <ostream>

#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/schema.h>
#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"

namespace takatori::statement {

/**
 * @brief index definition statement.
 */
class create_index final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr inline statement_kind tag { statement_kind::create_index };

    /**
     * @brief creates a new instance.
     * @param schema definition target schema
     * @param definition compile-time index information
     */
    explicit create_index(
            descriptor::schema schema,
            descriptor::relation definition) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit create_index(util::clone_tag_t, create_index const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit create_index(util::clone_tag_t, create_index&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] create_index* clone() const& override;
    [[nodiscard]] create_index* clone() && override;

    /**
     * @brief returns the target schema of this index definition.
     * @return the target schema
     */
    [[nodiscard]] descriptor::schema& schema() noexcept;

    /// @copydoc schema()
    [[nodiscard]] descriptor::schema const& schema() const noexcept;

    /**
     * @brief returns the compile-time index information.
     * @details The returned information is orphaned, that is,
     *      its index is not registered to any schemas.
     * @return the compile-time index information
     * @note Developers cannot change this information directly,
     *      please take a deep copy of this object.
     * @note The returned information may be incomplete:
     *      that does not consider the individual implementations.
     */
    [[nodiscard]] descriptor::relation& definition() noexcept;

    /// @copydoc definition()
    [[nodiscard]] descriptor::relation const& definition() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(create_index const& a, create_index const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(create_index const& a, create_index const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, create_index const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::schema schema_;
    descriptor::relation definition_;
};

} // namespace takatori::statement