#pragma once

#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "let_declarator.h"

#include "takatori/tree/tree_fragment_vector.h"

#include "takatori/util/object_creator.h"
#include "takatori/util/optional_ptr.h"
#include "takatori/util/rvalue_ptr.h"
#include "takatori/util/rvalue_reference_wrapper.h"
#include "takatori/util/meta_type.h"

namespace takatori::scalar {

/**
 * @brief a let expression.
 */
class let : public expression {
public:
    /// @brief the variable declarator type.
    using declarator = let_declarator;
    
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::let;

    let() = delete;
    ~let() override = default;
    let(let const& other) = delete;
    let& operator=(let const& other) = delete;
    let(let&& other) noexcept = delete;
    let& operator=(let&& other) noexcept = delete;

    /**
     * @brief creates a new object.
     * @param variables the variable declarations
     * @param body the body expression
     */
    explicit let(
            std::vector<declarator, util::pmr::polymorphic_allocator<declarator>> variables,
            util::unique_object_ptr<expression> body) noexcept;

    /**
     * @brief creates a new object.
     * @param variable the variable declaration
     * @param body the body expression
     * @attention this may take copies of given expressions
     */
    let(declarator&& variable, util::rvalue_ptr<expression> body);

    /**
     * @brief creates a new object.
     * @param variables the variable declarations
     * @param body the body expression
     * @attention this may take copies of given expressions
     */
    let(
            std::initializer_list<util::rvalue_reference_wrapper<declarator>> variables,
            util::rvalue_ptr<expression> body);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit let(let const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit let(let&& other, util::object_creator creator);

    parent_type* parent_element() noexcept override;
    parent_type const* parent_element() const noexcept override;
    void parent_element(parent_type* parent) noexcept override;

    expression_kind kind() const noexcept override;
    let* clone(util::object_creator creator) const& override;
    let* clone(util::object_creator creator) && override;

    /**
     * @brief returns the variables.
     * @return the variables
     */
    tree::tree_fragment_vector<declarator>& variables() noexcept;
    
    /// @copydoc variables()
    tree::tree_fragment_vector<declarator> const& variables() const noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     * @warning undefined behavior if the body is absent
     */
    expression& body() noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     * @warning undefined behavior if the body is absent
     */
    expression const& body() const noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     * @return empty if the body is absent
     */
    util::optional_ptr<expression> optional_body() noexcept;

    /// @copydoc optional_body()
    util::optional_ptr<expression const> optional_body() const noexcept;

    /**
     * @brief releases the body expression.
     * @return the body expression
     * @return empty if the body is absent
     */
    util::unique_object_ptr<expression> release_body() noexcept;

    /**
     * @brief sets the body expression.
     * @param body the replacement
     * @return this
     */
    let& body(util::unique_object_ptr<expression> body) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(let const& a, let const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(let const& a, let const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, let const& value);
    
protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    tree::tree_fragment_vector<declarator> variables_;
    util::unique_object_ptr<expression> body_;

    tree_element* parent_ {};
};

/**
 * @brief type_of for let.
 */
template<> struct type_of<let::tag> : util::meta_type<let> {};

} // namespace takatori::scalar
