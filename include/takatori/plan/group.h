#pragma once

#include <initializer_list>
#include <ostream>
#include <vector>

#include "exchange.h"
#include "step_kind.h"
#include "group_mode.h"

#include <takatori/relation/sort_direction.h>
#include <takatori/relation/details/sort_key_element.h>
#include <takatori/relation/details/row_slice.h>

#include <takatori/util/clone_tag.h>

namespace takatori::plan {

/**
 * @copydoc relation::sort_direction
 */
using sort_direction = relation::sort_direction;

/**
 * @brief exchange groups of rows.
 * @details This exchange produces group relations (the second order relations) as its output.
 *      Each downstream process must place @c take_group or @c take_cogroup operators to retrieve from such outputs,
 *      and use @c flatten_group to obtain individual rows.
 */
class group : public exchange {
public:
    /// @brief the sort key element.
    using sort_key = relation::details::sort_key_element;

    /// @brief the group mode type.
    using mode_type = group_mode;

    /// @brief the default group mode type.
    static constexpr mode_type mode_default = mode_type::equivalence;

    /// @brief row slice type.
    using row_slice_type = relation::details::row_slice;

    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::group;

    /**
     * @brief creates a new empty object.
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    group() = default;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param group_keys the group key columns: each column must also appear in the exchange columns
     * @param sort_keys the sort key elements: each column must also appear in the exchange columns, but not in group key columns
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of each group
     * @param mode the group mode of this exchange operation
     */
    explicit group(
            std::vector<descriptor::variable> columns,
            std::vector<descriptor::variable> group_keys,
            std::vector<sort_key> sort_keys = {},
            row_slice_type row_slice = {},
            mode_type mode = mode_default) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param group_keys the group key columns: each column must also appear in the exchange columns
     * @param sort_keys the sort key elements: each column must also appear in the exchange columns, but not in group key columns
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of each group
     * @param mode the group mode of this exchange operation
     */
    explicit group(
            std::initializer_list<descriptor::variable> columns,
            std::initializer_list<descriptor::variable> group_keys,
            std::initializer_list<sort_key> sort_keys = {},
            row_slice_type row_slice = {},
            mode_type mode = mode_default);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the copy source
     */
    explicit group(util::clone_tag_t, group const& other);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the move source
     */
    explicit group(util::clone_tag_t, group&& other);

    [[nodiscard]] step_kind kind() const noexcept override;
    [[nodiscard]] group* clone() const& override;
    [[nodiscard]] group* clone() && override;

    [[nodiscard]] util::sequence_view<descriptor::variable const> input_columns() const noexcept override;
    [[nodiscard]] util::sequence_view<descriptor::variable const> output_columns() const noexcept override;

    /**
     * @brief returns the columns to exchange.
     * @return the columns to exchange
     */
    [[nodiscard]] std::vector<descriptor::variable>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<descriptor::variable> const& columns() const noexcept;

    /**
     * @brief returns the group key columns.
     * @details each group key column must also appear in the columns(), but not appear in sort_keys().
     * @return the group key columns.
     */
    [[nodiscard]] std::vector<descriptor::variable>& group_keys() noexcept;

    /// @copydoc group_keys()
    [[nodiscard]] std::vector<descriptor::variable> const& group_keys() const noexcept;

    /**
     * @brief returns the sort key elements.
     * @details If the sort key is specified, this exchange will sort rows in each group by using it.
     *      Each sort key column must also appear in the columns(), but not appear in group_keys().
     *      If multiple sort key elements are specified, the earlier elements have higher priority.
     *      If there are out of slice rows in each group, they will be discarded after sorting.
     * @return the sort key elements
     */
    [[nodiscard]] std::vector<sort_key>& sort_keys() noexcept;

    /// @copydoc sort_keys()
    [[nodiscard]] std::vector<sort_key> const& sort_keys() const noexcept;

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
    group& mode(mode_type mode) noexcept;

    /**
     * @brief returns the row slice.
     * @details If this exchange has both the sort key and row slice,
     *      this exchange first performs sort rows in each group, and then slices rows in each group.
     * @return the row slice
     */
    [[nodiscard]] row_slice_type& row_slice() noexcept;

    /// @copydoc row_slice()
    [[nodiscard]] row_slice_type const& row_slice() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @details this don't compare upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(group const& a, group const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @details this don't compare upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(group const& a, group const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, group const& value);

protected:
    [[nodiscard]] bool equals(step const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::vector<descriptor::variable> columns_ {};
    std::vector<descriptor::variable> group_keys_ {};
    std::vector<sort_key> sort_keys_ {};
    row_slice_type row_slice_;
    mode_type mode_ { mode_default };
};

} // namespace takatori::plan
