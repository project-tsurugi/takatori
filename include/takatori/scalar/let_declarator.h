#pragma once

#include <iostream>

#include "variable_declarator.h"

namespace takatori::scalar {

class let;

/**
 * @brief a single variable declaration for "let" expression.
 * This satisfies "variable declarator" concept
 * @see let
 */
class let_declarator : private variable_declarator {
public:
    /// @brief the fragment owner type.
    using parent_type = let;

    using variable_declarator::variable_declarator;
    using variable_declarator::variable;
    using variable_declarator::value;
    using variable_declarator::optional_value;
    using variable_declarator::release_value;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit let_declarator(let_declarator const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit let_declarator(let_declarator&& other, util::object_creator creator);

    /**
     * @brief returns the owner of this fragment.
     * @return the owner
     * @return nullptr if it is absent
     */
    parent_type* parent_element() noexcept;

    /// @copydoc parent_element()
    parent_type const* parent_element() const noexcept;

    /**
     * @brief sets the owner of this fragment.
     * @param parent the owner
     */
    void parent_element(parent_type* parent) noexcept;

    /**
     * @brief sets the bound expression.
     * @param value the bound expression
     * @return this
     */
    let_declarator& value(util::unique_object_ptr<expression> value) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(let_declarator const& a, let_declarator const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(let_declarator const& a, let_declarator const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, let_declarator const& value);

private:
    parent_type* parent_ {};

    void fire_parent_element_changed() noexcept;

    friend class let;
};

} // namespace takatori::scalar
