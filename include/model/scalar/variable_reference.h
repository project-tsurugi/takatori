#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "expression_traits.h"

#include "model/descriptor/element_descriptor.h"

#include "util/meta_type.h"
#include "util/object_creator.h"

namespace model::scalar {

/**
 * @brief variable reference expression.
 */
class variable_reference final : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::variable_reference;

    variable_reference() = delete;
    ~variable_reference() override = default;
    variable_reference(variable_reference const& other) = delete;
    variable_reference& operator=(variable_reference const& other) = delete;
    variable_reference(variable_reference&& other) noexcept = delete;
    variable_reference& operator=(variable_reference&& other) noexcept = delete;

    /**
     * @brief creates a new object.
     * @param variable the descriptor of the target variable
     */
    variable_reference(descriptor::variable_descriptor variable) noexcept; // NOLINT

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    variable_reference(variable_reference const& other, util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    variable_reference(variable_reference&& other, util::object_creator creator) noexcept;

    parent_type* parent_element() noexcept override;
    parent_type const* parent_element() const noexcept override;
    void parent_element(parent_type* parent) noexcept override;;

    expression_kind kind() const noexcept override;
    variable_reference* clone(util::object_creator creator) const& override;
    variable_reference* clone(util::object_creator creator) && override;

    /**
     * @brief returns the descriptor of target variable.
     * @return the destination type
     */
    descriptor::variable_descriptor const& variable() const noexcept;

    /**
     * @brief sets a descriptor of target variable.
     * @param variable the target variable
     * @return this
     */
    variable_reference& variable(descriptor::variable_descriptor variable) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(variable_reference const& a, variable_reference const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend inline bool operator!=(variable_reference const& a, variable_reference const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, variable_reference const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::variable_descriptor variable_;
    parent_type* parent_ {};
};

/**
 * @brief expression_kind_type for variable_reference.
 */
template<> struct expression_kind_type<variable_reference::tag> : util::meta_type<variable_reference> {};

} // namespace model::scalar
