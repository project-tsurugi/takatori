#pragma once

#include <initializer_list>
#include <vector>

#include "../expression.h"
#include "../expression_kind.h"

#include "../details/aggregate_element.h"

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation::step {

/**
 * @brief aggregates columns in the input group.
 */
class aggregate : public expression {
public:
    /// @brief the individual columns aggregation.
    using column = details::aggregate_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::aggregate_group;

    /**
     * @brief creates a new instance.
     * @param columns the individual columns aggregation
     * @param creator the object creator for internal elements
     */
    explicit aggregate(
            std::vector<column, util::object_allocator<column>> columns,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new instance.
     * @param columns the individual columns aggregation
     */
    aggregate(std::initializer_list<column> columns);

    /**
     * @brief creates a new object.
     * @param other the copy destination
     * @param creator the object creator
     */
    explicit aggregate(aggregate const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move destination
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
     * @attention The input must be a group relation.
     * @return the input port
     * @see take_group
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
    std::vector<column, util::object_allocator<column>> columns_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for aggregate.
 */
template<> struct type_of<step::aggregate::tag> : util::meta_type<step::aggregate> {};

} // namespace takatori::relation
