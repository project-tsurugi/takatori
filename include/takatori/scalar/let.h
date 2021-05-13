#pragma once

#include <initializer_list>
#include <ostream>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "details/variable_declarator.h"

#include <takatori/tree/tree_fragment_vector.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/ownership_reference.h>
#include <takatori/util/rvalue_reference_wrapper.h>

namespace takatori::scalar {

/**
 * @brief a let expression.
 */
class let : public expression {
public:
    /// @brief the variable declarator type.
    using variable = details::variable_declarator<let>;

    /// @copydoc variable
    using declarator = variable;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::let;

    /**
     * @brief creates a new object.
     * @param variables the variable declarations
     * @param body the body expression
     */
    explicit let(
            std::vector<declarator> variables,
            std::unique_ptr<expression> body) noexcept;

    /**
     * @brief creates a new object.
     * @param variable the variable declaration
     * @param body the body expression
     * @attention this may take copies of given expressions
     */
    explicit let(declarator&& variable, expression&& body);

    /**
     * @brief creates a new object.
     * @param variables the variable declarations
     * @param body the body expression
     * @attention this may take copies of given expressions
     */
    explicit let(
            std::initializer_list<util::rvalue_reference_wrapper<declarator>> variables,
            expression&& body);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit let(util::clone_tag_t, let const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit let(util::clone_tag_t, let&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] let* clone() const& override;
    [[nodiscard]] let* clone() && override;

    /**
     * @brief returns the variables.
     * @return the variables
     */
    [[nodiscard]] tree::tree_fragment_vector<declarator>& variables() noexcept;
    
    /// @copydoc variables()
    [[nodiscard]] tree::tree_fragment_vector<declarator> const& variables() const noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     * @warning undefined behavior if the body is absent
     */
    [[nodiscard]] expression& body() noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     * @warning undefined behavior if the body is absent
     */
    [[nodiscard]] expression const& body() const noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     * @return empty if the body is absent
     */
    [[nodiscard]] util::optional_ptr<expression> optional_body() noexcept;

    /// @copydoc optional_body()
    [[nodiscard]] util::optional_ptr<expression const> optional_body() const noexcept;

    /**
     * @brief releases the body expression.
     * @return the body expression
     * @return empty if the body is absent
     */
    [[nodiscard]] std::unique_ptr<expression> release_body() noexcept;

    /**
     * @brief sets the body expression.
     * @param body the replacement
     * @return this
     */
    let& body(std::unique_ptr<expression> body) noexcept;

    /**
     * @brief returns ownership of the body expression.
     * @return the body expression
     */
    util::ownership_reference<expression> ownership_body() noexcept;

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
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    tree::tree_fragment_vector<declarator> variables_;
    std::unique_ptr<expression> body_;
};

/**
 * @brief type_of for let.
 */
template<> struct type_of<let::tag> : util::meta_type<let> {};

} // namespace takatori::scalar
