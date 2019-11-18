#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "expression.h"

#include "takatori/util/object_creator.h"
#include "takatori/util/optional_ptr.h"

namespace takatori::scalar {

class conditional;

/**
 * @brief an alternative clause in conditional expression.
 */
class conditional_alternative {
public:
    /// @brief the fragment owner type.
    using parent_type = conditional;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    conditional_alternative(conditional_alternative&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    conditional_alternative& operator=(conditional_alternative&& other) noexcept = default;

    /**
     * @brief creates a new instance.
     * @param condition the condition expression
     * @param body the body expression
     */
    explicit conditional_alternative(
            util::unique_object_ptr<expression> condition,
            util::unique_object_ptr<expression> body) noexcept;

    /**
     * @brief creates a new instance.
     * @param condition the condition expression
     * @param body the body expression
     * @attention this may take copies of given expressions
     */
    explicit conditional_alternative(
            expression&& condition,
            expression&& body) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit conditional_alternative(conditional_alternative const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit conditional_alternative(conditional_alternative&& other, util::object_creator creator);

    ~conditional_alternative() = default;

    conditional_alternative(conditional_alternative const& other) = delete;
    conditional_alternative& operator=(conditional_alternative const& other) = delete;

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
     * @brief returns the condition expression.
     * @return the condition expression
     */
    expression& condition() noexcept;
    
    /// @copydoc condition
    expression const& condition() const noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the condition expression is absent
     */
    util::optional_ptr<expression> optional_condition() noexcept;
    
    /// @copydoc optional_condition()
    util::optional_ptr<expression const> optional_condition() const noexcept;
    
    /**
     * @brief sets the condition expression.
     * @param condition the condition expression
     * @return this
     */
    conditional_alternative& condition(util::unique_object_ptr<expression> condition) noexcept;

    /**
     * @brief releases the condition expression.
     * @return the released expression
     */
    util::unique_object_ptr<expression> release_condition() noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     */
    expression& body() noexcept;

    /// @copydoc body
    expression const& body() const noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     * @return empty if the body expression is absent
     */
    util::optional_ptr<expression> optional_body() noexcept;

    /// @copydoc optional_body()
    util::optional_ptr<expression const> optional_body() const noexcept;

    /**
     * @brief sets the body expression.
     * @param body the body expression
     * @return this
     */
    conditional_alternative& body(util::unique_object_ptr<expression> body) noexcept;

    /**
     * @brief releases the body expression.
     * @return the released expression
     */
    util::unique_object_ptr<expression> release_body() noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(conditional_alternative const& a, conditional_alternative const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(conditional_alternative const& a, conditional_alternative const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, conditional_alternative const& value);

private:
    util::unique_object_ptr<expression> condition_;
    util::unique_object_ptr<expression> body_;

    parent_type* parent_ {};

    void fire_parent_element_changed() noexcept;

    friend class conditional;
};

} // namespace takatori::scalar
