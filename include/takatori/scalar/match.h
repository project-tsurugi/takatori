#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "expression_traits.h"
#include "match_operator.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::scalar {

/**
 * @brief pattern matching expression.
 */
class match final : public expression {
public:
    /// @brief match operator kind.
    using operator_kind_type = match_operator;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::match;

    match() = delete;
    ~match() override = default;
    match(match const& other) = delete;
    match& operator=(match const& other) = delete;
    match(match&& other) noexcept = delete;
    match& operator=(match&& other) noexcept = delete;

    /**
     * @brief creates a new object.
     * @param operator_kind the match operator kind
     * @param input the input sequence
     * @param pattern the pattern sequence
     * @param escape the escape character
     */
    explicit match(
            operator_kind_type operator_kind,
            util::unique_object_ptr<expression> input,
            util::unique_object_ptr<expression> pattern,
            util::unique_object_ptr<expression> escape) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the match operator kind
     * @param input the input sequence
     * @param pattern the pattern sequence
     * @param escape the escape character
     * @attention this may take copies of given expressions
     */
    match(
            operator_kind_type operator_kind,
            expression&& input,
            expression&& pattern,
            expression&& escape) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    match(match const& other, util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    match(match&& other, util::object_creator creator) noexcept;

    parent_type* parent_element() noexcept override;
    parent_type const* parent_element() const noexcept override;
    void parent_element(parent_type* parent) noexcept override;;

    expression_kind kind() const noexcept override;
    match* clone(util::object_creator creator) const& override;
    match* clone(util::object_creator creator) && override;

    /**
     * @brief returns the operator kind.
     * @return operator kind
     */
    operator_kind_type operator_kind() const noexcept;

    /**
     * @brief sets operator kind.
     * @param operator_kind operator kind
     * @return this
     */
    match& operator_kind(operator_kind_type operator_kind) noexcept;

    /**
     * @brief returns the input sequence.
     * @return the input sequence
     * @warning undefined behavior if the operand is absent
     */
    expression& input() noexcept;

    /**
     * @brief returns the input sequence.
     * @return the input sequence
     * @warning undefined behavior if the input is absent
     */
    expression const& input() const noexcept;

    /**
     * @brief returns the input sequence.
     * @return the input sequence
     * @return empty if the input is absent
     */
    util::optional_ptr<expression> optional_input() noexcept;

    /// @copydoc optional_operand()
    util::optional_ptr<expression const> optional_input() const noexcept;

    /**
     * @brief releases the input sequence.
     * @return the input sequence
     * @return empty if the input is absent
     */
    util::unique_object_ptr<expression> release_input() noexcept;

    /**
     * @brief sets the input sequence.
     * @param input the replacement
     * @return this
     */
    match& input(util::unique_object_ptr<expression> input) noexcept;

    /**
     * @brief returns the pattern sequence.
     * @return the pattern sequence
     * @warning undefined behavior if the pattern is absent
     */
    expression& pattern() noexcept;

    /**
     * @brief returns the pattern sequence.
     * @return the pattern sequence
     * @warning undefined behavior if the pattern is absent
     */
    expression const& pattern() const noexcept;

    /**
     * @brief returns the pattern sequence.
     * @return the pattern sequence
     * @return empty if the pattern is absent
     */
    util::optional_ptr<expression> optional_pattern() noexcept;

    /// @copydoc optional_operand()
    util::optional_ptr<expression const> optional_pattern() const noexcept;

    /**
     * @brief releases the pattern sequence.
     * @return the pattern sequence
     * @return empty if the pattern is absent
     */
    util::unique_object_ptr<expression> release_pattern() noexcept;

    /**
     * @brief sets the pattern sequence.
     * @param pattern the replacement
     * @return this
     */
    match& pattern(util::unique_object_ptr<expression> pattern) noexcept;

    /**
     * @brief returns the escape character.
     * @return the escape character
     * @warning undefined behavior if the expression is absent
     */
    expression& escape() noexcept;

    /**
     * @brief returns the escape character.
     * @return the escape character
     * @warning undefined behavior if the expression is absent
     */
    expression const& escape() const noexcept;

    /**
     * @brief returns the escape character.
     * @return the escape character
     * @return empty if the expression is absent
     */
    util::optional_ptr<expression> optional_escape() noexcept;

    /// @copydoc optional_operand()
    util::optional_ptr<expression const> optional_escape() const noexcept;

    /**
     * @brief releases the escape character.
     * @return the escape character
     * @return empty if the expression is absent
     */
    util::unique_object_ptr<expression> release_escape() noexcept;

    /**
     * @brief sets the escape character.
     * @param escape the replacement
     * @return this
     */
    match& escape(util::unique_object_ptr<expression> escape) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(match const& a, match const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend inline bool operator!=(match const& a, match const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, match const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    operator_kind_type operator_kind_;
    util::unique_object_ptr<expression> input_;
    util::unique_object_ptr<expression> pattern_;
    util::unique_object_ptr<expression> escape_;
    parent_type* parent_ {};
};

/**
 * @brief expression_kind_type for match.
 */
template<> struct expression_kind_type<match::tag> : util::meta_type<match> {};

} // namespace takatori::scalar
