#pragma once

#include <array>

#include "../expression.h"
#include "../expression_kind.h"
#include "../join_kind.h"

#include <takatori/scalar/expression.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/rvalue_ptr.h>

namespace takatori::relation::intermediate {

/**
 * @brief performs join on two input relations.
 */
class join : public expression {
public:
    /// @brief the join direction
    using operator_kind_type = relation::join_kind;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::join_relation;

    /**
     * @brief creates a new object.
     * @param operator_kind the join kind
     * @param condition the join condition expression, or null to perform cross join
     * @param creator the object creator for internal elements
     */
    explicit join(
            operator_kind_type operator_kind,
            util::unique_object_ptr<scalar::expression> condition,
            util::object_creator creator = {}) noexcept;

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
     * @param creator the object creator
     */
    explicit join(join const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit join(join&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    join* clone(util::object_creator creator) const& override;
    join* clone(util::object_creator creator) && override;

    /**
     * @brief returns the left input port.
     * @return the left input port
     */
    input_port_type& left() noexcept;

    /// @copydoc left()
    input_port_type const& left() const noexcept;

    /**
     * @brief returns the right input port.
     * @return the right input port
     */
    input_port_type& right() noexcept;

    /// @copydoc right()
    input_port_type const& right() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    output_port_type& output() noexcept;

    /// @copydoc output()
    output_port_type const& output() const noexcept;

    /**
     * @brief returns the join kind.
     * @return the join kind
     */
    operator_kind_type operator_kind() const noexcept;

    /**
     * @brief sets the join kind.
     * @param operator_kind the join kind
     * @return this
     */
    join& operator_kind(operator_kind_type operator_kind) noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::optional_ptr<scalar::expression> condition() noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::optional_ptr<scalar::expression const> condition() const noexcept;

    /**
     * @brief releases the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::unique_object_ptr<scalar::expression> release_condition() noexcept;

    /**
     * @brief sets the condition expression.
     * @param condition the replacement
     * @return this
     */
    join& condition(util::unique_object_ptr<scalar::expression> condition) noexcept;

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
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::array<input_port_type, 2> inputs_;
    output_port_type output_;
    operator_kind_type operator_kind_;
    util::unique_object_ptr<scalar::expression> condition_;

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

