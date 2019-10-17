#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "expression_traits.h"
#include "unary_operator.h"

#include "util/meta_type.h"
#include "util/object_creator.h"

namespace model::scalar {

/**
 * @brief unary expression.
 */
class unary final : public expression {
public:
    /// @brief unary operator kind.
    using operator_kind_type = unary_operator;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::unary;

    unary() = delete;
    ~unary() override = default;
    unary(unary const& other) = delete;
    unary& operator=(unary const& other) = delete;
    unary(unary&& other) noexcept = delete;
    unary& operator=(unary&& other) noexcept = delete;

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
    unary(
            operator_kind_type operator_kind,
            expression&& operand) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    unary(unary const& other, util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    unary(unary&& other, util::object_creator creator) noexcept;

    parent_type* parent_element() noexcept override;
    parent_type const* parent_element() const noexcept override;
    void parent_element(parent_type* parent) noexcept override;;

    expression_kind kind() const noexcept override;
    unary* clone(util::object_creator creator) const& override;
    unary* clone(util::object_creator creator) && override;

    /**
     * @brief returns the operator kind.
     * @return operator kind
     */
    operator_kind_type operator_kind() const noexcept;

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
    expression const& operand() const noexcept;

    /**
     * @brief returns the expression operand.
     * @return the expression operand
     * @return empty if the operand is absent
     */
    util::optional_ptr<expression> optional_operand() noexcept;

    /// @copydoc optional_operand()
    util::optional_ptr<expression const> optional_operand() const noexcept;

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
    friend inline bool operator!=(unary const& a, unary const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, unary const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    operator_kind_type operator_kind_;
    util::unique_object_ptr<expression> operand_;
    parent_type* parent_ {};
};

/**
 * @brief expression_kind_type for unary.
 */
template<> struct expression_kind_type<unary::tag> : util::meta_type<unary> {};

} // namespace model::scalar
