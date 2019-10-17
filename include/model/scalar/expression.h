#pragma once

#include <iostream>

#include "expression_kind.h"

#include "model/tree_element.h"

#include "util/object_creator.h"
#include "util/optional_ptr.h"

/**
 * @brief scalar expression models for SQL IR
 */
namespace model::scalar {

/**
 * @brief a root model of scalar expressions.
 */
class expression : public tree_element {
public:
    /// @brief the parent type.
    using parent_type = tree_element;

    /**
     * @brief creates a new instance.
     */
    expression() = default;

    /**
     * @brief destroys this object.
     */
    ~expression() override = default;

    /**
     * @brief returns the kind of this expression.
     * @return the expression kind
     */
    virtual expression_kind kind() const noexcept = 0;

    /**
     * @brief returns a clone of this object.
     * @param creator the object creator
     * @return the created clone
     */
    virtual expression* clone(util::object_creator creator) const& = 0;

    /// @copydoc clone()
    virtual expression* clone(util::object_creator creator) && = 0;

    using tree_element::parent_element;

    /**
     * @brief sets the parent element.
     * @param parent the parent element
     */
    virtual void parent_element(parent_type* parent) noexcept = 0;

    /**
     * @brief returns the parent expression
     * @return the parent expression
     * @return empty if it is not set, or the parent element is not an expression
     */
    util::optional_ptr<expression> parent_expression() noexcept;

    /// @copydoc parent_expression()
    util::optional_ptr<expression const> parent_expression() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(expression const& a, expression const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend inline bool operator!=(expression const& a, expression const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, expression const& value);

protected:
    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    expression(expression const& other) = default;

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    expression& operator=(expression const& other) = default;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    expression(expression&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    expression& operator=(expression&& other) noexcept = default;

    /**
     * @brief returns whether or not this expressions is equivalent to the target one.
     * @param other the target expression
     * @return true if the both are equivalent
     * @return false otherwise
     */
    virtual bool equals(expression const& other) const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;
};

} // namespace model::scalar
