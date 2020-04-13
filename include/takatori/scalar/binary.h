#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "binary_operator.h"

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/ownership_reference.h>

namespace takatori::scalar {

/**
 * @brief binary expression.
 */
class binary final : public expression {
public:
    /// @brief binary operator kind.
    using operator_kind_type = binary_operator;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::binary;

    /**
     * @brief creates a new object.
     * @param operator_kind the binary operator kind
     * @param left the left operand
     * @param right the right operand
     */
    explicit binary(
            operator_kind_type operator_kind,
            util::unique_object_ptr<expression> left,
            util::unique_object_ptr<expression> right) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the binary operator kind
     * @param left the left operand
     * @param right the right operand
     * @attention this may take copies of given expressions
     */
    explicit binary(
            operator_kind_type operator_kind,
            expression&& left,
            expression&& right) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit binary(binary const& other, util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit binary(binary&& other, util::object_creator creator) noexcept;

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] binary* clone(util::object_creator creator) const& override;
    binary* clone(util::object_creator creator) && override;

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
    binary& operator_kind(operator_kind_type operator_kind) noexcept;

    /**
     * @brief returns the left operand.
     * @return the left operand
     * @warning undefined behavior if the operand is absent
     */
    expression& left() noexcept;

    /**
     * @brief returns the left operand.
     * @return the left operand
     * @warning undefined behavior if the operand is absent
     */
    [[nodiscard]] expression const& left() const noexcept;

    /**
     * @brief returns the left operand.
     * @return the left operand
     * @return empty if the operand is absent
     */
    util::optional_ptr<expression> optional_left() noexcept;

    /// @copydoc optional_left()
    [[nodiscard]] util::optional_ptr<expression const> optional_left() const noexcept;

    /**
     * @brief releases the left operand.
     * @return the left operand
     * @return empty if the operand is absent
     */
    util::unique_object_ptr<expression> release_left() noexcept;

    /**
     * @brief sets the left operand.
     * @param left the replacement
     * @return this
     */
    binary& left(util::unique_object_ptr<expression> left) noexcept;

    /**
     * @brief returns ownership reference of the left operand.
     * @return the left operand
     */
    util::object_ownership_reference<expression> ownership_left();

    /**
     * @brief returns the right operand.
     * @return the right operand
     * @warning undefined behavior if the operand is absent
     */
    expression& right() noexcept;

    /**
     * @brief returns the right operand.
     * @return the right operand
     * @warning undefined behavior if the operand is absent
     */
    [[nodiscard]] expression const& right() const noexcept;

    /**
     * @brief returns the right operand.
     * @return the right operand
     * @return empty if the operand is absent
     */
    util::optional_ptr<expression> optional_right() noexcept;

    /// @copydoc optional_right()
    [[nodiscard]] util::optional_ptr<expression const> optional_right() const noexcept;

    /**
     * @brief releases the right operand.
     * @return the right operand
     * @return empty if the operand is absent
     */
    util::unique_object_ptr<expression> release_right() noexcept;

    /**
     * @brief sets the right operand.
     * @param right the replacement
     * @return this
     */
    binary& right(util::unique_object_ptr<expression> right) noexcept;

    /**
     * @brief returns ownership reference of the right operand.
     * @return the right operand
     */
    util::object_ownership_reference<expression> ownership_right();

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(binary const& a, binary const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(binary const& a, binary const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, binary const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    operator_kind_type operator_kind_;
    util::unique_object_ptr<expression> left_;
    util::unique_object_ptr<expression> right_;
};

/**
 * @brief type_of for binary.
 */
template<> struct type_of<binary::tag> : util::meta_type<binary> {};

} // namespace takatori::scalar
