#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "unary_operator.h"

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/ownership_reference.h>

namespace takatori::scalar {

/**
 * @brief unary expression.
 */
class unary final : public expression {
public:
    /// @brief unary operator kind.
    using operator_kind_type = unary_operator;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::unary;

    /**
     * @brief creates a new object.
     * @param operator_kind the unary operator kind
     * @param operand the expression operand
     */
    explicit unary(
            operator_kind_type operator_kind,
            util::unique_object_ptr<expression> operand) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the unary operator kind
     * @param operand the expression operand
     * @attention this may take copies of given expressions
     */
    explicit unary(
            operator_kind_type operator_kind,
            expression&& operand) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit unary(unary const& other, util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit unary(unary&& other, util::object_creator creator) noexcept;

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] unary* clone(util::object_creator creator) const& override;
    unary* clone(util::object_creator creator) && override;

    /**
     * @brief returns the operator kind.
     * @return operator kind
     */
    [[nodiscard]] operator_kind_type operator_kind() const noexcept;

    /**
     * @brief sets operator kind.
     * @param operator_kind operator kind
     * @return this
     */
    unary& operator_kind(operator_kind_type operator_kind) noexcept;

    /**
     * @brief returns the expression operand.
     * @return the expression operand
     * @warning undefined behavior if the operand is absent
     */
    expression& operand() noexcept;

    /**
     * @brief returns the expression operand.
     * @return the expression operand
     * @warning undefined behavior if the operand is absent
     */
    [[nodiscard]] expression const& operand() const noexcept;

    /**
     * @brief returns the expression operand.
     * @return the expression operand
     * @return empty if the operand is absent
     */
    util::optional_ptr<expression> optional_operand() noexcept;

    /// @copydoc optional_operand()
    [[nodiscard]] util::optional_ptr<expression const> optional_operand() const noexcept;

    /**
     * @brief releases the expression operand.
     * @return the expression operand
     * @return empty if the operand is absent
     */
    util::unique_object_ptr<expression> release_operand() noexcept;

    /**
     * @brief sets the expression operand.
     * @param operand the replacement
     * @return this
     */
    unary& operand(util::unique_object_ptr<expression> operand) noexcept;

    /**
     * @brief returns ownership reference of the expression operand.
     * @return the expression operand
     */
    util::object_ownership_reference<expression> ownership_operand();

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(unary const& a, unary const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(unary const& a, unary const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, unary const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    operator_kind_type operator_kind_;
    util::unique_object_ptr<expression> operand_;
};

/**
 * @brief type_of for unary.
 */
template<> struct type_of<unary::tag> : util::meta_type<unary> {};

} // namespace takatori::scalar
