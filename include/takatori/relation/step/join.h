#pragma once

#include "../expression.h"
#include "../expression_kind.h"
#include "../join_kind.h"

#include "takatori/scalar/expression.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"
#include "takatori/util/rvalue_ptr.h"

namespace takatori::relation::step {

/**
 * @brief join groups (co-group).
 * @details The resulting rows must be first order.
 * If the number of input groups is just two, this operation treats as the first group is left input, and
 * the second group is right input.
 * If the number of input groups is greater than two, this only supports the join kind of inner and full outer.
 * @li inner - emits output rows only if the all groups are not empty,
 *             and the extra condition is absent or evaluated as true.
 * @li left_outer - emits output rows only if the first group is not empty,
 *                  and columns from such the empty groups becomes null,
 *                  or if the extra condition is present and evaluated as not true then the all columns
 *                  except from the first group becomes null.
 * @li full_outer - always emits output rows even if some input groups are empty,
 *                  and columns from such the empty groups becomes null.
 *                  the extra condition is unsupported.
 * @li semi - emits columns in the first group only if the all groups are not empty,
 *            and the extra condition is absent or evaluated as true
 * @li anti - emits columns in the first group only if neither the any groups except the first one are empty,
 *            nor the extra condition is present and evaluated as true
 */
class join : public expression {
public:
    /// @brief the join direction
    using operator_kind_type = relation::join_kind;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::join_group;

    /**
     * @brief creates a new instance.
     * @param operator_kind the join kind
     * @param condition the extra join condition expression
     * @param creator the object creator for internal elements
     */
    explicit join(
            operator_kind_type operator_kind,
            util::unique_object_ptr<scalar::expression> condition,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the join kind
     * @param condition the extra join condition expression
     */
    explicit join(
            operator_kind_type operator_kind,
            util::rvalue_ptr<scalar::expression> condition = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy destination
     * @param creator the object creator
     */
    explicit join(join const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move destination
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
     * @brief returns the input port.
     * @attention The input must be a co-group relation.
     * @return the input port
     * @see take_cogroup
     */
    input_port_type& input() noexcept;

    /// @copydoc input()
    input_port_type const& input() const noexcept;

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
     * @brief returns the extra condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::optional_ptr<scalar::expression> condition() noexcept;

    /**
     * @brief returns the extra condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::optional_ptr<scalar::expression const> condition() const noexcept;

    /**
     * @brief releases the extra condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::unique_object_ptr<scalar::expression> release_condition() noexcept;

    /**
     * @brief sets the extra condition expression.
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
    input_port_type input_;
    output_port_type output_;
    operator_kind_type operator_kind_;
    util::unique_object_ptr<scalar::expression> condition_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for step::join.
 */
template<> struct type_of<step::join::tag> : util::meta_type<step::join> {};

} // namespace takatori::relation
