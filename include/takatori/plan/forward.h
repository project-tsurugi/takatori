#pragma once

#include <initializer_list>
#include <ostream>
#include <vector>

#include <takatori/relation/details/row_slice.h>

#include <takatori/util/clone_tag.h>

#include "exchange.h"
#include "step_kind.h"

namespace takatori::plan {

/**
 * @brief exchange just the input data.
 */
class forward : public exchange {
public:
    /// @brief row slice type.
    using row_slice_type = relation::details::row_slice;

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
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    explicit forward(row_slice_type row_slice) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of the relation
     */
    explicit forward(
            std::vector<descriptor::variable> columns,
            row_slice_type row_slice = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of the relation
     */
    forward(
            std::initializer_list<descriptor::variable> columns,
            row_slice_type row_slice = {}) noexcept;

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
     * @brief returns the row slice.
     * @details if there are out of the slice rows, they will be discarded.
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
};

} // namespace takatori::plan
