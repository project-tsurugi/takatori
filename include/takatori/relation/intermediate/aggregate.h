#pragma once

#include <initializer_list>
#include <vector>

#include "../expression.h"
#include "../expression_kind.h"
#include "../details/aggregate_element.h"

#include "takatori/descriptor/variable.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::relation::intermediate {

/**
 * @brief aggregates columns in the input relation.
 * @attention if the grouping keys are absent, this organizes an empty group for the empty input
 */
class aggregate : public expression {
public:
    /// @brief the individual columns aggregation.
    using column = details::aggregate_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::aggregate_relation;

    /**
     * @brief creates a new object.
     * @param keys the grouping key columns on the input relation, or empty if aggregates whole the input relation
     * @param columns the individual columns aggregation
     * @param creator the object creator for internal elements
     */
    explicit aggregate(
            std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> keys,
            std::vector<column, util::object_allocator<column>> columns,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param keys the grouping key columns on the input relation
     * @param columns the individual columns aggregation
     */
    explicit aggregate(
            std::initializer_list<descriptor::variable> keys,
            std::initializer_list<column> columns);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit aggregate(aggregate const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit aggregate(aggregate&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    aggregate* clone(util::object_creator creator) const& override;
    aggregate* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * @return the input port
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
     * @brief returns the aggregate key columns on the input relation.
     * @return the aggregate keys
     * @return empty if this operator aggregates the whole input relation as a single group
     */
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& keys() noexcept;

    /// @copydoc keys()
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& keys() const noexcept;

    /**
     * @brief returns the individual columns aggregation.
     * @return the aggregation specification of each column
     */
    std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(aggregate const& a, aggregate const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(aggregate const& a, aggregate const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, aggregate const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> keys_;
    std::vector<column, util::object_allocator<column>> columns_;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for aggregate.
 */
template<> struct type_of<intermediate::aggregate::tag> : util::meta_type<intermediate::aggregate> {};

} // namespace takatori::relation

