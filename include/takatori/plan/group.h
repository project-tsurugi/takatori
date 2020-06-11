#pragma once

#include <initializer_list>
#include <ostream>
#include <optional>
#include <utility>
#include <vector>

#include "exchange.h"
#include "step_kind.h"
#include "group_mode.h"

#include <takatori/relation/sort_direction.h>
#include <takatori/relation/details/sort_key_element.h>

#include <takatori/util/object_creator.h>

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

    /// @brief the size type.
    using size_type = std::size_t;

    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::group;

    /**
     * @brief creates a new empty object.
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    group() = default;

    /**
     * @brief creates a new object.
     * @param creator the object creator for internal elements
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    explicit group(util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param group_keys the group key columns: each column must also appear in the exchange columns
     * @param sort_keys the sort key elements: each column must also appear in the exchange columns, but not in group key columns
     * @param limit the max number of rows in each group
     * @param mode the group mode of this exchange operation
     * @param creator the object creator for internal elements
     */
    explicit group(
            std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> columns,
            std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> group_keys,
            std::vector<sort_key, util::object_allocator<sort_key>> sort_keys = {},
            std::optional<size_type> limit = {},
            mode_type mode = mode_default,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param group_keys the group key columns: each column must also appear in the exchange columns
     * @param sort_keys the sort key elements: each column must also appear in the exchange columns, but not in group key columns
     * @param limit the max number of rows in each group
     * @param mode the group mode of this exchange operation
     */
    explicit group(
            std::initializer_list<descriptor::variable> columns,
            std::initializer_list<descriptor::variable> group_keys,
            std::initializer_list<sort_key> sort_keys = {},
            std::optional<size_type> limit = {},
            mode_type mode = mode_default);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit group(group const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the move source
     * @param creator the object creator
     */
    explicit group(group&& other, util::object_creator creator);

    [[nodiscard]] step_kind kind() const noexcept override;
    [[nodiscard]] group* clone(util::object_creator creator) const& override;
    [[nodiscard]] group* clone(util::object_creator creator) && override;

    [[nodiscard]] util::sequence_view<descriptor::variable const> input_columns() const noexcept override;
    [[nodiscard]] util::sequence_view<descriptor::variable const> output_columns() const noexcept override;

    /**
     * @brief returns the columns to exchange.
     * @return the columns to exchange
     */
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& columns() const noexcept;

    /**
     * @brief returns the group key columns.
     * @details each group key column must also appear in the columns(), but not appear in sort_keys().
     * @return the group key columns.
     */
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& group_keys() noexcept;

    /// @copydoc group_keys()
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& group_keys() const noexcept;

    /**
     * @brief returns the sort key elements.
     * @details If the sort key is specified, this exchange will sort rows in each group by using it.
     *      Each sort key column must also appear in the columns(), but not appear in group_keys().
     * @return the sort key elements
     */
    [[nodiscard]] std::vector<sort_key, util::object_allocator<sort_key>>& sort_keys() noexcept;

    /// @copydoc sort_keys()
    [[nodiscard]] std::vector<sort_key, util::object_allocator<sort_key>> const& sort_keys() const noexcept;

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
     * @brief returns the max number of exchange rows in each group.
     * @details If this exchange has both the limit and sort key,
     *      this exchange first performs sort rows in each group,
     *      and then removes rows except the first n-th rows in each group.
     * @return the number of exchange rows in each group
     * @return empty if it is unlimited
     */
    [[nodiscard]] std::optional<size_type> const& limit() const noexcept;

    /**
     * @brief sets the max number of exchange rows in each group.
     * @param limit the number of exchange rows limit
     * @return this
     * @see limit()
     */
    group& limit(std::optional<size_type> limit) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details this don't compares upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(group const& a, group const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details this don't compares upstream processes nor downstream processes.
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
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> columns_ {};
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> group_keys_ {};
    std::vector<sort_key, util::object_allocator<sort_key>> sort_keys_ {};
    std::optional<size_type> limit_ {};
    mode_type mode_ { mode_default };
};

} // namespace takatori::plan
