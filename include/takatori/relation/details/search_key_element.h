#pragma once

#include <iostream>

#include <takatori/descriptor/variable.h>

#include <takatori/scalar/expression.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/object_creator.h>
#include <takatori/util/optional_ptr.h>

namespace takatori::relation::details {

/**
 * @brief a search key to find for rows in relations.
 * @tparam Parent the fragment owner type
 */
template<class Parent>
class search_key_element {
public:
    /// @brief the fragment owner type.
    using parent_type = Parent;

    /**
     * @brief creates a new instance.
     * @param variable the search key column on the target relation
     * @param value the search value for the target column
     */
    explicit search_key_element(
            descriptor::variable variable,
            util::unique_object_ptr<scalar::expression> value) noexcept
        : variable_(std::move(variable))
        , value_(std::move(value))
    {}


    /**
     * @brief creates a new instance.
     * @param variable the search key column on the target relation
     * @param value the search value for the target column
     * @attention this may take a copy of arguments
     */
    explicit search_key_element(
            descriptor::variable variable, // FIXME: NOLINT(performance-unnecessary-value-param)
            scalar::expression&& value) noexcept
        : search_key_element(
                std::move(variable),
                util::clone_unique(std::move(value)))
    {}


    ~search_key_element() = default;

    search_key_element(search_key_element const& other) = delete;
    search_key_element& operator=(search_key_element const& other) = delete;
    
    /**
     * @brief creates a new object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    search_key_element(search_key_element&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    search_key_element& operator=(search_key_element&& other) noexcept = default;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit search_key_element(search_key_element const& other, util::object_creator creator)
        : search_key_element(
                other.variable_,
                tree::forward(creator, other.value_))
    {}

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit search_key_element(search_key_element&& other, util::object_creator creator)
        : search_key_element(
                std::move(other.variable_),
                tree::forward(creator, std::move(other.value_)))
    {}

    /**
     * @brief returns the owner of this fragment.
     * @return the owner
     * @return nullptr if it is absent
     */
    parent_type* parent_element() noexcept {
        return parent_;
    }

    /// @copydoc parent_element()
    parent_type const* parent_element() const noexcept {
        return parent_;
    }

    /**
     * @brief sets the owner of this fragment.
     * @param parent the owner
     */
    void parent_element(parent_type* parent) noexcept {
        parent_ = parent;
        tree::bless_element(util::optional_ptr { parent_ }, value_);
    }

    /**
     * @brief returns the search key column on the target relation.
     * @return the variable descriptor
     */
    descriptor::variable& variable() noexcept {
        return variable_;
    }

    /// @copydoc variable()
    descriptor::variable const& variable() const noexcept {
        return variable_;
    }

    /**
     * @brief returns the search value for the target column.
     * @return the search value
     */
    scalar::expression& value() noexcept {
        return *value_;
    }

    /// @copydoc value
    scalar::expression const& value() const noexcept {
        return *value_;
    }

    /**
     * @brief returns the search value for the target column.
     * @return the search value
     * @return empty if the expression is absent
     */
    util::optional_ptr<scalar::expression> optional_value() noexcept {
        return util::optional_ptr { value_.get() };
    }

    /// @copydoc optional_value()
    util::optional_ptr<scalar::expression const> optional_value() const noexcept {
        return util::optional_ptr { value_.get() };
    }

    /**
     * @brief releases the search value for the target column.
     * @return the search value
     * @return empty if the expression is absent
     */
    util::unique_object_ptr<scalar::expression> release_value() noexcept {
        return tree::release_element(std::move(value_));
    }

    /**
     * @brief sets the search value for the target column.
     * @param value the search value
     * @return this
     */
    search_key_element& value(util::unique_object_ptr<scalar::expression> value) noexcept {
        tree::assign_element_fragment(parent_, value_, std::move(value));
        return *this;
    }

private:
    descriptor::variable variable_;
    util::unique_object_ptr<scalar::expression> value_;
    parent_type* parent_ {};
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
template<class Parent>
inline bool
operator==(search_key_element<Parent> const& a, search_key_element<Parent> const& b) noexcept {
    return a.variable() == b.variable()
        && a.optional_value() == b.optional_value();
}

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
template<class Parent>
inline bool
operator!=(search_key_element<Parent> const& a, search_key_element<Parent> const& b) noexcept {
    return !(a == b);
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<class Parent>
inline std::ostream&
operator<<(std::ostream& out, search_key_element<Parent> const& value) {
    return out << "key_element("
               << "variable=" << value.variable() << ", "
               << "value=" << value.optional_value() << ")";
}

} // namespace takatori::relation::details
