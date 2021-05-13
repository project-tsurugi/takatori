#pragma once

#include <array>
#include <initializer_list>
#include <memory>
#include <vector>
#include <utility>

#include "../expression.h"
#include "../expression_kind.h"
#include "../join_kind.h"

#include "../details/search_key_element.h"
#include "../details/range_endpoint.h"

#include <takatori/scalar/expression.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>
#include <takatori/util/ownership_reference.h>
#include <takatori/util/rvalue_ptr.h>

namespace takatori::relation::intermediate {

/**
 * @brief performs join on two input relations.
 */
class join : public expression {
public:
    /// @brief the join direction
    using operator_kind_type = relation::join_kind;

    /// @brief the range key piece type.
    using key = details::search_key_element<join>;

    /// @brief the range end-point specification
    using endpoint = details::range_endpoint<join, key>;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::join_relation;

    /**
     * @brief creates a new object.
     * @details This constructor accepts the "full" properties, including optimizer information,
     *      and designed for the compiler internals.
     *      To make building objects easier, please use other constructors.
     * @param operator_kind the join kind
     * @param lower the lower end-point specification.
     *      The each key must be a column of the left input relation,
     *      and each `value` cannot include any variables came from the left input
     * @param upper the upper end-point specification.
     *      The each key must be a column of the left input relation,
     *      and each `value` cannot include any variables came from the left input
     * @param condition the extra join condition expression,
     * @note `lower` and `upper` are designed for optimizers.
     *      If `lower` or `upper` are present, optimizers will generate a more effective execution plan for its bounds.
     *      Otherwise, the operator will be replaced with a simple nested loop join.
     */
    explicit join(
            operator_kind_type operator_kind,
            endpoint lower,
            endpoint upper,
            std::unique_ptr<scalar::expression> condition) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the join kind
     * @param condition the join condition expression, or null to perform cross join
     */
    explicit join(
            operator_kind_type operator_kind,
            std::unique_ptr<scalar::expression> condition) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the join kind
     * @param condition the join condition expression, or empty to perform cross join
     */
    explicit join(
            operator_kind_type operator_kind,
            util::rvalue_ptr<scalar::expression> condition = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit join(util::clone_tag_t, join const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit join(util::clone_tag_t, join&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] join* clone() const& override;
    [[nodiscard]] join* clone() && override;

    /**
     * @brief returns the left input port.
     * @return the left input port
     */
    [[nodiscard]] input_port_type& left() noexcept;

    /// @copydoc left()
    [[nodiscard]] input_port_type const& left() const noexcept;

    /**
     * @brief returns the right input port.
     * @return the right input port
     */
    [[nodiscard]] input_port_type& right() noexcept;

    /// @copydoc right()
    [[nodiscard]] input_port_type const& right() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the join kind.
     * @return the join kind
     */
    [[nodiscard]] operator_kind_type operator_kind() const noexcept;

    /**
     * @brief sets the join kind.
     * @param operator_kind the join kind
     * @return this
     */
    join& operator_kind(operator_kind_type operator_kind) noexcept;

    /**
     * @brief returns the lower end-point specification.
     * @return the lower end-point specification
     */
    [[nodiscard]] endpoint& lower() noexcept;

    /// @copydoc lower()
    [[nodiscard]] endpoint const& lower() const noexcept;

    /**
     * @brief returns the upper end-point specification.
     * @return the upper end-point specification
     */
    [[nodiscard]] endpoint& upper() noexcept;

    /// @copydoc upper()
    [[nodiscard]] endpoint const& upper() const noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    [[nodiscard]] util::optional_ptr<scalar::expression> condition() noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    [[nodiscard]] util::optional_ptr<scalar::expression const> condition() const noexcept;

    /**
     * @brief releases the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    [[nodiscard]] std::unique_ptr<scalar::expression> release_condition() noexcept;

    /**
     * @brief sets the condition expression.
     * @param condition the replacement
     * @return this
     */
    join& condition(std::unique_ptr<scalar::expression> condition) noexcept;

    /**
     * @brief returns ownership of the condition expression.
     * @return the condition expression
     */
    [[nodiscard]] util::ownership_reference<scalar::expression> ownership_condition() noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(join const& a, join const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(join const& a, join const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, join const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::array<input_port_type, 2> inputs_;
    output_port_type output_;
    operator_kind_type operator_kind_;
    endpoint lower_;
    endpoint upper_;
    std::unique_ptr<scalar::expression> condition_;

    static inline constexpr std::size_t left_index = 0;
    static inline constexpr std::size_t right_index = left_index + 1;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for join.
 */
template<> struct type_of<intermediate::join::tag> : util::meta_type<intermediate::join> {};

} // namespace takatori::relation

