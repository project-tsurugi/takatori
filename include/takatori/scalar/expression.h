#pragma once

#include <memory>
#include <ostream>

#include "expression_kind.h"

#include <takatori/document/region.h>

#include <takatori/tree/tree_element_base.h>

#include <takatori/util/optional_ptr.h>

namespace takatori::scalar {

/**
 * @brief a root model of scalar expressions.
 */
class expression : public tree::tree_element_base {
public:
    /// @brief the parent type.
    using parent_type = tree::tree_element_base;

    /**
     * @brief creates a new instance.
     */
    expression() = default;

    /**
     * @brief destroys this object.
     */
    ~expression() override = default;

    expression(expression const& other) = delete;
    expression& operator=(expression const& other) = delete;
    expression(expression&& other) noexcept = delete;
    expression& operator=(expression&& other) noexcept = delete;

    /**
     * @brief returns the kind of this expression.
     * @return the expression kind
     */
    [[nodiscard]] virtual expression_kind kind() const noexcept = 0;

    /**
     * @brief returns a clone of this object.
     * @return the created clone
     */
    [[nodiscard]] virtual expression* clone() const& = 0;

    /// @copydoc clone()
    [[nodiscard]] virtual expression* clone() && = 0;

    /**
     * @brief returns the parent element.
     * @return the parent element
     * @return nullptr if this is the root or orphaned element
     */
    [[nodiscard]] parent_type* parent_element() noexcept;

    /**
     * @brief returns the parent element.
     * @return the parent element
     * @return nullptr if this is the root or orphaned element
     */
    [[nodiscard]] parent_type const* parent_element() const noexcept;

    /**
     * @brief sets the parent element.
     * @param parent the parent element
     */
    void parent_element(parent_type* parent) noexcept;

    /**
     * @brief returns the parent expression
     * @return the parent expression
     * @return empty if it is not set, or the parent element is not an expression
     */
    [[nodiscard]] util::optional_ptr<expression> parent_expression() noexcept;

    /// @copydoc parent_expression()
    [[nodiscard]] util::optional_ptr<expression const> parent_expression() const noexcept;

    /**
     * @brief returns the document region of this element.
     * @return the document region
     */
    [[nodiscard]] document::region& region() noexcept;

    /// @copydoc region()
    [[nodiscard]] document::region const& region() const noexcept;

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
    friend bool operator!=(expression const& a, expression const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, expression const& value);

protected:
    /**
     * @brief returns whether or not this expressions is equivalent to the target one.
     * @param other the target expression
     * @return true if the both are equivalent
     * @return false otherwise
     */
    [[nodiscard]] virtual bool equals(expression const& other) const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;

private:
    parent_type* parent_ {};
    document::region region_ {};
};

} // namespace takatori::scalar
