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

class let;

/**
 * @brief a single variable declaration for "let" expression.
 * @see let
 */
class let_declarator {
public:
    /// @brief the fragment owner type.
    using parent_type = let;

    let_declarator() = delete;
    ~let_declarator() = default;

    let_declarator(let_declarator const& other) = delete;
    let_declarator& operator=(let_declarator const& other) = delete;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    let_declarator(let_declarator&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    let_declarator& operator=(let_declarator&& other) noexcept = default;

    /**
     * @brief creates a new instance.
     * @param variable the descriptor of the declared variable
     * @param value the bound expression
     */
    explicit let_declarator(
            descriptor::variable variable,
            util::unique_object_ptr<expression> value) noexcept;

    /**
     * @brief creates a new instance.
     * @param variable the descriptor of the declared variable
     * @param value the bound expression
     * @attention this may take copies of given expressions
     */
    let_declarator(
            descriptor::variable variable,
            expression&& value) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    let_declarator(let_declarator const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    let_declarator(let_declarator&& other, util::object_creator creator);

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
     * @brief returns the descriptor of target variable.
     * @return the destination type
     */
    descriptor::variable const& variable() const noexcept;

    /**
     * @brief sets a descriptor of the declared variable.
     * @param variable the declared variable
     * @return this
     */
    let_declarator& variable(descriptor::variable variable) noexcept;

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
     * @brief sets the bound expression.
     * @param value the bound expression
     * @return this
     */
    let_declarator& value(util::unique_object_ptr<expression> value) noexcept;

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
    descriptor::variable variable_;
    util::unique_object_ptr<expression> value_;

    parent_type* parent_ {};

    void fire_parent_element_changed() noexcept;

    friend class let;
};

} // namespace takatori::scalar
