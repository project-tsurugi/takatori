#pragma once

#include <iostream>

#include <takatori/scalar/expression.h>

#include <takatori/descriptor/variable.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/optional_ptr.h>

namespace takatori::scalar::details {

/**
 * @brief a single variable declaration.
 * @tparam Parent the parent type
 */
template<class Parent>
class variable_declarator {
public:
    /// @brief the fragment owner type.
    using parent_type = Parent;

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
     */
    explicit variable_declarator(
            util::unique_object_ptr<expression> value,
            descriptor::variable variable) noexcept;

    /**
     * @brief creates a new instance.
     * @param value the bound expression
     * @param variable the descriptor of the declared variable
     * @attention this may take a copy of the expression
     */
    explicit variable_declarator(
            descriptor::variable variable,
            expression&& value) noexcept;

    /**
     * @brief creates a new instance.
     * @param value the bound expression
     * @param variable the descriptor of the declared variable
     * @attention this may take a copy of the expression
     */
    explicit variable_declarator(
            expression&& value,
            descriptor::variable variable) noexcept;

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
     * @return the target variable
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
     * @brief sets the bound expression.
     * @param value the bound expression
     * @return this
     */
    variable_declarator& value(util::unique_object_ptr<expression> value) noexcept;

private:
    descriptor::variable variable_;
    util::unique_object_ptr<expression> value_;
    parent_type* parent_ {};
};

template<class Parent>
inline
variable_declarator<Parent>::variable_declarator(
        descriptor::variable variable,
        util::unique_object_ptr<expression> value) noexcept
    : variable_(std::move(variable))
    , value_(std::move(value))
{}

template<class Parent>
inline
variable_declarator<Parent>::variable_declarator(
        util::unique_object_ptr<expression> value,
        descriptor::variable variable) noexcept
    : variable_(std::move(variable))
    , value_(std::move(value))
{}

template<class Parent>
inline
variable_declarator<Parent>::variable_declarator(
        descriptor::variable variable, // FIXME: NOLINT(performance-unnecessary-value-param)
        expression&& value) noexcept
    : variable_declarator(
        std::move(variable),
        util::clone_unique(std::move(value)))
{}

template<class Parent>
inline
variable_declarator<Parent>::variable_declarator(
        expression&& value,
        descriptor::variable variable) noexcept // FIXME: NOLINT(performance-unnecessary-value-param)
    : variable_declarator(
        std::move(variable),
        util::clone_unique(std::move(value)))
{}

template<class Parent>
inline
variable_declarator<Parent>::variable_declarator(
        variable_declarator const& other,
        util::object_creator creator)
    : variable_declarator(
        other.variable_,
        tree::forward(creator, other.value_))
{}

template<class Parent>
inline
variable_declarator<Parent>::variable_declarator(
        variable_declarator&& other,
        util::object_creator creator)
    : variable_declarator(
        std::move(other.variable_),
        tree::forward(creator, std::move(other.value_)))
{}

template<class Parent>
inline typename variable_declarator<Parent>::parent_type*
variable_declarator<Parent>::parent_element() noexcept {
    return parent_;
}

template<class Parent>
inline typename variable_declarator<Parent>::parent_type
const* variable_declarator<Parent>::parent_element() const noexcept {
    return parent_;
}

template<class Parent>
inline void
variable_declarator<Parent>::parent_element(parent_type* parent) noexcept {
    parent_ = parent;
    tree::bless_element(parent_, value_);
}

template<class Parent>
inline descriptor::variable const&
variable_declarator<Parent>::variable() const noexcept {
    return variable_;
}

template<class Parent>
inline variable_declarator<Parent>&
variable_declarator<Parent>::variable(descriptor::variable variable) noexcept { // FIXME: NOLINT(performance-unnecessary-value-param)
    variable_ = std::move(variable);
    return *this;
}

template<class Parent>
inline expression&
variable_declarator<Parent>::value() noexcept {
    return *value_;
}

template<class Parent>
inline expression const&
variable_declarator<Parent>::value() const noexcept {
    return *value_;
}

template<class Parent>
inline util::optional_ptr<expression>
variable_declarator<Parent>::optional_value() noexcept {
    return util::optional_ptr { value_.get() };
}

template<class Parent>
inline util::optional_ptr<expression const>
variable_declarator<Parent>::optional_value() const noexcept {
    return util::optional_ptr { value_.get() };
}

template<class Parent>
inline util::unique_object_ptr<expression>
variable_declarator<Parent>::release_value() noexcept {
    return tree::release_element(std::move(value_));
}

template<class Parent>
inline variable_declarator<Parent>&
variable_declarator<Parent>::value(util::unique_object_ptr<expression> value) noexcept {
    tree::assign_element_fragment(parent_, variable_, std::move(value));
    return *this;
}

/**
 * @brief returns whether or not the two elements are equivalent.
 * @tparam Parent the parent type
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
template<class Parent>
inline bool
operator==(variable_declarator<Parent> const& a, variable_declarator<Parent> const& b) noexcept {
    return a.variable() == b.variable()
        && a.optional_value() == b.optional_value();
}

/**
 * @brief returns whether or not the two elements are different.
 * @tparam Parent the parent type
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
template<class Parent>
inline bool
operator!=(variable_declarator<Parent> const& a, variable_declarator<Parent> const& b) noexcept {
    return !(a == b);
}

/**
 * @brief appends string representation of the given value.
 * @tparam Parent the parent type
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<class Parent>
inline std::ostream&
operator<<(std::ostream& out, variable_declarator<Parent> const& value) {
    return out << "variable_declarator" << "("
               << "variable=" << value.variable() << ", "
               << "value=" << value.optional_value() << ")";
}

} // namespace takatori::scalar::details
