#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "match_operator.h"

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>
#include <takatori/util/ownership_reference.h>

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
    /**
     * @brief creates a new object.
     * @param operator_kind the match operator kind
     * @param input the input sequence
     * @param pattern the pattern sequence
     * @param escape the escape character
     */
    explicit match(
            operator_kind_type operator_kind,
            std::unique_ptr<expression> input,
            std::unique_ptr<expression> pattern,
            std::unique_ptr<expression> escape) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the match operator kind
     * @param input the input sequence
     * @param pattern the pattern sequence
     * @param escape the escape character
     * @attention this may take copies of given expressions
     */
    explicit match(
            operator_kind_type operator_kind,
            expression&& input,
            expression&& pattern,
            expression&& escape) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit match(util::clone_tag_t, match const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit match(util::clone_tag_t, match&& other) noexcept;

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] match* clone() const& override;
    [[nodiscard]] match* clone() && override;

    /**
     * @brief returns the operator kind.
     * @return operator kind
     */
    [[nodiscard]] operator_kind_type operator_kind() const noexcept;

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
    [[nodiscard]] expression& input() noexcept;

    /**
     * @brief returns the input sequence.
     * @return the input sequence
     * @warning undefined behavior if the input is absent
     */
    [[nodiscard]] expression const& input() const noexcept;

    /**
     * @brief returns the input sequence.
     * @return the input sequence
     * @return empty if the input is absent
     */
    [[nodiscard]] util::optional_ptr<expression> optional_input() noexcept;

    /// @copydoc optional_input()
    [[nodiscard]] util::optional_ptr<expression const> optional_input() const noexcept;

    /**
     * @brief releases the input sequence.
     * @return the input sequence
     * @return empty if the input is absent
     */
    [[nodiscard]] std::unique_ptr<expression> release_input() noexcept;

    /**
     * @brief sets the input sequence.
     * @param input the replacement
     * @return this
     */
    match& input(std::unique_ptr<expression> input) noexcept;

    /**
     * @brief returns ownership reference of the input sequence.
     * @return the input sequence
     */
    [[nodiscard]] util::ownership_reference<expression> ownership_input();

    /**
     * @brief returns the pattern sequence.
     * @return the pattern sequence
     * @warning undefined behavior if the pattern is absent
     */
    [[nodiscard]] expression& pattern() noexcept;

    /**
     * @brief returns the pattern sequence.
     * @return the pattern sequence
     * @warning undefined behavior if the pattern is absent
     */
    [[nodiscard]] expression const& pattern() const noexcept;

    /**
     * @brief returns the pattern sequence.
     * @return the pattern sequence
     * @return empty if the pattern is absent
     */
    [[nodiscard]] util::optional_ptr<expression> optional_pattern() noexcept;

    /// @copydoc optional_pattern()
    [[nodiscard]] util::optional_ptr<expression const> optional_pattern() const noexcept;

    /**
     * @brief releases the pattern sequence.
     * @return the pattern sequence
     * @return empty if the pattern is absent
     */
    [[nodiscard]] std::unique_ptr<expression> release_pattern() noexcept;

    /**
     * @brief sets the pattern sequence.
     * @param pattern the replacement
     * @return this
     */
    match& pattern(std::unique_ptr<expression> pattern) noexcept;

    /**
     * @brief returns ownership reference of the pattern sequence.
     * @return the pattern sequence
     */
    [[nodiscard]] util::ownership_reference<expression> ownership_pattern();

    /**
     * @brief returns the escape character.
     * @return the escape character
     * @warning undefined behavior if the expression is absent
     */
    [[nodiscard]] expression& escape() noexcept;

    /**
     * @brief returns the escape character.
     * @return the escape character
     * @warning undefined behavior if the expression is absent
     */
    [[nodiscard]] expression const& escape() const noexcept;

    /**
     * @brief returns the escape character.
     * @return the escape character
     * @return empty if the expression is absent
     */
    [[nodiscard]] util::optional_ptr<expression> optional_escape() noexcept;

    /// @copydoc optional_escape()
    [[nodiscard]] util::optional_ptr<expression const> optional_escape() const noexcept;

    /**
     * @brief releases the escape character.
     * @return the escape character
     * @return empty if the expression is absent
     */
    [[nodiscard]] std::unique_ptr<expression> release_escape() noexcept;

    /**
     * @brief sets the escape character.
     * @param escape the replacement
     * @return this
     */
    match& escape(std::unique_ptr<expression> escape) noexcept;

    /**
     * @brief returns ownership reference of the escape character.
     * @return the escape character
     */
    [[nodiscard]] util::ownership_reference<expression> ownership_escape();

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
    friend bool operator!=(match const& a, match const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, match const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    operator_kind_type operator_kind_;
    std::unique_ptr<expression> input_;
    std::unique_ptr<expression> pattern_;
    std::unique_ptr<expression> escape_;
};

/**
 * @brief type_of for match.
 */
template<> struct type_of<match::tag> : util::meta_type<match> {};

} // namespace takatori::scalar
