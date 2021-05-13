#pragma once

#include <initializer_list>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "details/conditional_alternative.h"
#include <takatori/tree/tree_fragment_vector.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/rvalue_ptr.h>
#include <takatori/util/rvalue_reference_wrapper.h>
#include <takatori/util/meta_type.h>
#include <takatori/util/ownership_reference.h>

namespace takatori::scalar {

/**
 * @brief a conditional expression.
 */
class conditional : public expression {
public:
    /// @brief alternative type.
    using alternative = details::conditional_alternative;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::conditional;

    /**
     * @brief creates a new object.
     * @param alternatives the case alternatives
     * @param default_expression the default case expression
     */
    explicit conditional(
            std::vector<alternative> alternatives,
            std::unique_ptr<expression> default_expression) noexcept;

    /**
     * @brief creates a new object.
     * @param alternatives the case alternatives
     * @param default_expression the default case expression
     * @attention this may take copies of given expressions
     */
    conditional(
            std::initializer_list<util::rvalue_reference_wrapper<alternative>> alternatives,
            util::rvalue_ptr<expression> default_expression = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit conditional(util::clone_tag_t, conditional const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit conditional(util::clone_tag_t, conditional&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] conditional* clone() const& override;
    [[nodiscard]] conditional* clone() && override;

    /**
     * @brief returns the alternatives.
     * @return the alternatives
     */
    [[nodiscard]] tree::tree_fragment_vector<alternative>& alternatives() noexcept;
    
    /// @copydoc alternatives()
    [[nodiscard]] tree::tree_fragment_vector<alternative> const& alternatives() const noexcept;

    /**
     * @brief returns the default expression.
     * @return the default_expression expression
     * @return empty if the default expression is absent
     */
    [[nodiscard]] util::optional_ptr<expression> default_expression() noexcept;

    /// @copydoc default_expression()
    [[nodiscard]] util::optional_ptr<expression const> default_expression() const noexcept;

    /**
     * @brief sets the default expression.
     * @param default_expression the default expression
     * @return this
     */
    conditional& default_expression(std::unique_ptr<expression> default_expression) noexcept;

    /**
     * @brief returns ownership reference of the default expression.
     * @return the default expression
     */
    [[nodiscard]] util::ownership_reference<expression> ownership_default_expression();

    /**
     * @brief releases the default_expression expression.
     * @return the released expression
     * @return empty if the default expression is absent
     */
    [[nodiscard]] std::unique_ptr<expression> release_default_expression() noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(conditional const& a, conditional const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(conditional const& a, conditional const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, conditional const& value);
    
protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    tree::tree_fragment_vector<alternative> alternatives_;
    std::unique_ptr<expression> default_expression_;
};

/**
 * @brief type_of for conditional.
 */
template<> struct type_of<conditional::tag> : util::meta_type<conditional> {};

} // namespace takatori::scalar
