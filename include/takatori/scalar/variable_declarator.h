#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "expression.h"

#include "takatori/descriptor/variable.h"

#include "takatori/util/object_creator.h"
#include "takatori/util/optional_ptr.h"

namespace takatori::scalar {

/**
 * @brief a single variable declaration.
 * @see let
 */
class variable_declarator {
public:
    /**
     * @brief creates a new instance.
     * @param variable the descriptor of the declared variable
     * @param value the bound expression
     */
    explicit variable_declarator(
            descriptor::variable variable,
            util::unique_object_ptr<expression> value) noexcept;

    /**
     * @brief creates a new instance.
     * @param variable the descriptor of the declared variable
     * @param value the bound expression
     * @attention this may take a copy of the expression
     */
    explicit variable_declarator(
            descriptor::variable variable,
            expression&& value) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit variable_declarator(variable_declarator const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit variable_declarator(variable_declarator&& other, util::object_creator creator);

    ~variable_declarator() = default;

    variable_declarator(variable_declarator const& other) = delete;
    variable_declarator& operator=(variable_declarator const& other) = delete;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    variable_declarator(variable_declarator&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    variable_declarator& operator=(variable_declarator&& other) noexcept = default;

    /**
     * @brief returns the descriptor of target variable.
     * @return the destination type
     */
    descriptor::variable const& variable() const noexcept;

    /**
     * @brief sets a descriptor of the declared variable.
     * @param variable the declared variable
     * @return this
     */
    variable_declarator& variable(descriptor::variable variable) noexcept;

    /**
     * @brief returns the bound expression.
     * @return the bound expression
     */
    expression& value() noexcept;

    /// @copydoc value
    expression const& value() const noexcept;

    /**
     * @brief returns the bound expression.
     * @return the bound expression
     * @return empty if the bound expression is absent
     */
    util::optional_ptr<expression> optional_value() noexcept;

    /// @copydoc optional_value()
    util::optional_ptr<expression const> optional_value() const noexcept;

    /**
     * @brief releases the bound expression.
     * @return the released expression
     */
    util::unique_object_ptr<expression> release_value() noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(variable_declarator const& a, variable_declarator const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(variable_declarator const& a, variable_declarator const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, variable_declarator const& value);

protected:
    /**
     * @brief returns the container of bound expression.
     * @return the bound expression container
     */
    util::unique_object_ptr<expression>& internal_value() noexcept;

    /**
     * @brief returns whether or not this and the given element is equivalent.
     * @param other the target element
     * @return true if the both are equivalent
     * @return false otherwise
     */
    bool equals(variable_declarator const& other) const noexcept;

    /**
     * @brief appends string representation of this.
     * @param out the target output
     * @return the output
     */
    std::ostream& print_to(std::ostream& out) const;

private:
    descriptor::variable variable_;
    util::unique_object_ptr<expression> value_;
};

} // namespace takatori::scalar
