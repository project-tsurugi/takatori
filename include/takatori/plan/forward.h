#pragma once

#include <initializer_list>
#include <ostream>
#include <vector>

#include <takatori/relation/details/row_slice.h>
#include <takatori/relation/details/row_slice_compat.h>

#include <takatori/util/clone_tag.h>

#include "exchange.h"
#include "forward_ordering.h"
#include "step_kind.h"

namespace takatori::plan {

/**
 * @brief exchange just the input data.
 */
class forward : public exchange {
public:
    /// @brief row slice type.
    using row_slice_type = relation::details::row_slice;

    /// @brief the ordering properties type.
    using ordering_type = forward_ordering_set;

    /**
     * @brief the size type.
     * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
     */
    using size_type = std::size_t;

    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::forward;

    /**
     * @brief creates a new object.
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    forward() = default;

    /**
     * @brief creates a new object.
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of the relation
     * @param ordering the properties about the row order
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    explicit forward(
            row_slice_type row_slice,
            ordering_type ordering = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of the relation
     * @param ordering the properties about the row order
     */
    explicit forward(
            std::vector<descriptor::variable> columns,
            row_slice_type row_slice = {},
            ordering_type ordering = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of the relation
     * @param ordering the properties about the row order
     */
    forward(
            std::initializer_list<descriptor::variable> columns,
            row_slice_type row_slice = {},
            ordering_type ordering = {}) noexcept;

    /**
     * @brief creates a new object.
     * @tparam T the row count type, must be an integral type
     * @param limit the max number of rows to forward
     * @note the created object does not have any exchange columns, please define them using columns()
     * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
     */
    template<class T, relation::details::enable_if_compatible_row_count_t<T> = nullptr>
    explicit forward(T limit) noexcept :
        forward {
                row_slice_type { {}, static_cast<size_type>(limit) },
        }
    {}

    /**
     * @brief creates a new object.
     * @tparam T the row count type, must be an integral type
     * @param columns the exchange columns
     * @param limit the max number of rows to forward
     * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
     */
    template<class T, relation::details::enable_if_compatible_row_count_t<T> = nullptr>
    explicit forward(
            std::vector<descriptor::variable> columns,
            T limit) noexcept :
        forward {
                std::move(columns),
                row_slice_type { {}, static_cast<size_type>(limit) },
        }
    {}

    /**
     * @brief creates a new object.
     * @tparam T the row count type, must be an integral type
     * @param columns the exchange columns
     * @param limit the max number of rows to forward
     * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
     */
    template<class T, relation::details::enable_if_compatible_row_count_t<T> = nullptr>
    forward(
            std::initializer_list<descriptor::variable> columns,
            T limit) noexcept :
        forward {
                columns,
                row_slice_type { {}, static_cast<size_type>(limit) },
        }
    {}

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the copy source
     */
    explicit forward(util::clone_tag_t, forward const& other);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the move source
     */
    explicit forward(util::clone_tag_t, forward&& other);

    [[nodiscard]] step_kind kind() const noexcept override;
    [[nodiscard]] forward* clone() const& override;
    [[nodiscard]] forward* clone() && override;

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
     * @brief returns the row slice of this exchange.
     * @details This exchange only passes the rows in the slice of the whole input
     *      (the outputs of all upstream processes together) to the downstream.
     *      If ordering() contains forward_ordering::respect_input and the input relation is an ordered relation,
     *      the slice is taken in the order defined by the input relation.
     *      Otherwise, the input rows are not ordered: which rows are passed is not specified,
     *      and only the number of rows is defined.
     * @return the row slice
     * @see ordering()
     */
    [[nodiscard]] row_slice_type& row_slice() noexcept;

    /// @copydoc row_slice()
    [[nodiscard]] row_slice_type const& row_slice() const noexcept;

    /**
     * @brief returns the properties about the row order of this exchange.
     * @details Each property is effective only if the input relation is an ordered relation.
     *      If this is empty, this exchange does not consider the row order.
     *      Note that the concatenation of two or more input relations is not an ordered relation.
     * @return the ordering properties
     * @see forward_ordering
     */
    [[nodiscard]] ordering_type& ordering() noexcept;

    /// @copydoc ordering()
    [[nodiscard]] ordering_type const& ordering() const noexcept;

    /**
     * @brief returns the max number of exchange rows.
     * @return the number of exchange rows limit
     * @return empty if it is unlimited
     * @throws std::domain_error if row_slice() has the start row position other than immediate `0`
     * @throws std::domain_error if row_slice() has a variable row count
     * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
     */
    [[nodiscard]] std::optional<size_type> limit() const;

    /**
     * @brief sets the max number of exchange rows.
     * @details This only changes the row count of row_slice(), and keeps its start row position.
     * @param limit the number of exchange rows limit, or empty to unlimited
     * @return this
     * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
     */
    forward& limit(std::optional<size_type> limit) noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @details this don't compare upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(forward const& a, forward const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @details this don't compare upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(forward const& a, forward const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, forward const& value);

protected:
    [[nodiscard]] bool equals(step const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::vector<descriptor::variable> columns_ {};
    row_slice_type row_slice_;
    ordering_type ordering_ {};
};

} // namespace takatori::plan
