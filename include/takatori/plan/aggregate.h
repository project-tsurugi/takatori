#pragma once

#include <initializer_list>
#include <ostream>
#include <optional>
#include <utility>
#include <vector>

#include "exchange.h"
#include "step_kind.h"
#include "group_mode.h"

#include <takatori/relation/details/aggregate_element.h>

#include <takatori/util/object_creator.h>

namespace takatori::plan {

/**
 * @brief aggregates groups into rows.
 * @details This exchange produces group relations (the second order relations) as its output,
 *      and each group has upto a single row (for optimization).
 *      Each downstream process must place @c take_group or @c take_cogroup operators to retrieve from such outputs,
 *      and use @c flatten_group to obtain individual rows.
 */
class aggregate : public exchange {
public:
    /// @brief the individual columns aggregation.
    using aggregation = relation::details::aggregate_element;

    /// @brief the group mode type.
    using mode_type = group_mode;

    /// @brief the default group mode type.
    static constexpr mode_type mode_default = mode_type::equivalence;

    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::aggregate;

    /**
     * @brief creates a new empty object.
     */
    aggregate() = default;

    /**
     * @brief creates a new object.
     * @param creator the object creator for internal elements
     */
    explicit aggregate(util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param source_columns the input columns of this exchange: the upstream processes offer to these columns
     * @param destination_columns the output columns of this exchange:
     *      the downstream processes take from these columns.
     *      these columns must also appear in the @c group_keys or destination of @c aggregations
     * @param group_keys the group key columns: each column must also appear in the @em source columns,
     *      and may or may not appear in the @em destination columns
     * @param aggregations the individual aggregation operations:
     *      each aggregate function argument must be also appear in @em source columns
     * @param mode the group mode of this exchange operation
     * @param creator the object creator for internal elements
     */
    explicit aggregate(
            std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> source_columns,
            std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> destination_columns,
            std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> group_keys,
            std::vector<aggregation, util::object_allocator<aggregation>> aggregations,
            mode_type mode = mode_default,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @details this automatically organizes source_columns() and destination_columns() from the arguments as the following:
     *      @li @c source_columns() - from @c group_keys and arguments of @c aggregations,
     *      @li @c destination_columns() - from @c group_keys and destination of @c aggregations
     *      Note that the duplicated columns are removed.
     * @param group_keys the group key columns
     * @param aggregations the individual aggregation operations
     * @param mode the group mode of this exchange operation
     */
    explicit aggregate(
            std::initializer_list<descriptor::variable> group_keys,
            std::initializer_list<aggregation> aggregations,
            mode_type mode = mode_default);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit aggregate(aggregate const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the move source
     * @param creator the object creator
     */
    explicit aggregate(aggregate&& other, util::object_creator creator);

    [[nodiscard]] step_kind kind() const noexcept override;
    [[nodiscard]] aggregate* clone(util::object_creator creator) const& override;
    [[nodiscard]] aggregate* clone(util::object_creator creator) && override;

    [[nodiscard]] util::sequence_view<descriptor::variable const> input_columns() const noexcept override;
    [[nodiscard]] util::sequence_view<descriptor::variable const> output_columns() const noexcept override;

    /**
     * @brief returns the source columns.
     * @details These columns are used for group_keys() or arguments of aggregations()
     * @return the source columns
     */
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& source_columns() noexcept;

    /// @copydoc source_columns()
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& source_columns() const noexcept;

    /**
     * @brief returns the destination columns .
     * @details These columns must come from group_keys() or destination of aggregations()
     * @return the destination columns
     */
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& destination_columns() noexcept;

    /// @copydoc destination_columns()
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& destination_columns() const noexcept;

    /**
     * @brief returns the aggregate key columns.
     * @details each aggregate key column must also appear in the source_columns().
     * @return the aggregate key columns.
     */
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& group_keys() noexcept;

    /// @copydoc group_keys()
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& group_keys() const noexcept;

    /**
     * @brief returns the aggregation elements that contain aggregation function, its arguments, and destination column.
     * @details individual arguments must come from source_columns()
     * @return the aggregation elements
     */
    [[nodiscard]] std::vector<aggregation, util::object_allocator<aggregation>>& aggregations() noexcept;

    /// @copydoc aggregations()
    [[nodiscard]] std::vector<aggregation, util::object_allocator<aggregation>> const& aggregations() const noexcept;

    /**
     * @brief returns group mode of this exchange operation.
     * @return the group mode
     */
    [[nodiscard]] mode_type mode() const noexcept;

    /**
     * @brief sets the group mode.
     * @param mode the group mode
     * @return this
     */
    aggregate& mode(mode_type mode) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details this don't compares upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(aggregate const& a, aggregate const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details this don't compares upstream processes nor downstream processes.
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
    [[nodiscard]] bool equals(step const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> source_columns_ {};
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> destination_columns_ {};
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> group_keys_ {};
    std::vector<aggregation, util::object_allocator<aggregation>> aggregations_ {};
    mode_type mode_ { mode_default };
};

} // namespace takatori::plan
