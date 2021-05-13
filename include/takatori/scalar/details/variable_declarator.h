#pragma once

#include <ostream>

#include <takatori/scalar/expression.h>

#include <takatori/descriptor/variable.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/clone_tag.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/ownership_reference.h>

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
            std::unique_ptr<expression> value) noexcept
        : variable_(std::move(variable))
        , value_(std::move(value))
    {}

    /**
     * @brief creates a new instance.
     * @param variable the descriptor of the declared variable
     * @param value the bound expression
     */
    explicit variable_declarator(
            std::unique_ptr<expression> value,
            descriptor::variable variable) noexcept
        : variable_(std::move(variable))
        , value_(std::move(value))
    {}

    /**
     * @brief creates a new instance.
     * @param value the bound expression
     * @param variable the descriptor of the declared variable
     * @attention this may take a copy of the expression
     */
    explicit variable_declarator(
            descriptor::variable variable, // FIXME: NOLINT(performance-unnecessary-value-param)
            expression&& value) noexcept
        : variable_declarator(
                std::move(variable),
                util::clone_unique(std::move(value)))
    {}

    /**
     * @brief creates a new instance.
     * @param value the bound expression
     * @param variable the descriptor of the declared variable
     * @attention this may take a copy of the expression
     */
    explicit variable_declarator(
            expression&& value,
            descriptor::variable variable) noexcept // FIXME: NOLINT(performance-unnecessary-value-param)
        : variable_declarator(
                std::move(variable),
                util::clone_unique(std::move(value)))
    {}

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
     */
    explicit variable_declarator(util::clone_tag_t, variable_declarator const& other)
        : variable_declarator(
                other.variable_,
                tree::forward(other.value_))
    {}

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit variable_declarator(util::clone_tag_t, variable_declarator&& other)
        : variable_declarator(
                std::move(other.variable_),
                tree::forward(std::move(other.value_)))
    {}

    /**
     * @brief returns the owner of this fragment.
     * @return the owner
     * @return nullptr if it is absent
     */
    [[nodiscard]] parent_type* parent_element() noexcept {
        return parent_;
    }

    /// @copydoc parent_element()
    [[nodiscard]] parent_type const* parent_element() const noexcept {
        return parent_;
    }

    /**
     * @brief sets the owner of this fragment.
     * @param parent the owner
     */
    void parent_element(parent_type* parent) noexcept {
        parent_ = parent;
        tree::bless_element(parent_, value_);
    }

    /**
     * @brief returns the descriptor of target variable.
     * @return the target variable
     */
    [[nodiscard]] descriptor::variable& variable() noexcept {
        return variable_;
    }

    /// @copydoc variable()
    [[nodiscard]] descriptor::variable const& variable() const noexcept {
        return variable_;
    }

    /**
     * @brief returns the bound expression.
     * @return the bound expression
     */
    [[nodiscard]] expression& value() noexcept {
        return *value_;
    }

    /// @copydoc value
    [[nodiscard]] expression const& value() const noexcept {
        return *value_;
    }

    /**
     * @brief returns the bound expression.
     * @return the bound expression
     * @return empty if the bound expression is absent
     */
    [[nodiscard]] util::optional_ptr<expression> optional_value() noexcept {
        return util::optional_ptr { value_.get() };
    }

    /// @copydoc optional_value()
    [[nodiscard]] util::optional_ptr<expression const> optional_value() const noexcept {
        return util::optional_ptr { value_.get() };
    }

    /**
     * @brief releases the bound expression.
     * @return the released expression
     */
    [[nodiscard]] std::unique_ptr<expression> release_value() noexcept {
        return tree::release_element(std::move(value_));
    }

    /**
     * @brief sets the bound expression.
     * @param value the bound expression
     * @return this
     */
    variable_declarator& value(std::unique_ptr<expression> value) noexcept {
        tree::assign_element_fragment(parent_, variable_, std::move(value));
        return *this;
    }

    /**
     * @brief returns ownership of the bound expression.
     * @return the bound expression
     */
    [[nodiscard]] util::ownership_reference<expression> ownership_value() {
        return tree::ownership_element_fragment(parent_, variable_);
    }

private:
    descriptor::variable variable_;
    std::unique_ptr<expression> value_;
    parent_type* parent_ {};
};

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
