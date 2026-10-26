#pragma once

#include <cstddef>
#include <ostream>

#include "constant_value.h"

namespace takatori::relation::details {

/**
 * @brief represents a slice of rows in a relation.
 * @details A slice consists of the rows that remain after skipping the first `start` rows of the relation,
 *      and then taking at most `count` rows from the rest.
 *      That is, it consists of the rows at the 1-origin positions from `start + 1` to `start + count`, if they exist,
 *      and its number of rows is `min(count, max(0, N - start))`, where `N` is the number of rows in the relation.
 *
 *      Each of `start` and `count` is empty, an immediate value, or a variable:
 *      - empty `start` skips no rows, and empty `count` does not restrict the number of rows
 *      - `count` of `0` takes no rows
 *      - a variable whose value is `NULL` is treated as same as empty
 *      - a negative value of a variable raises an error at runtime
 *      - the value of a variable is determined before the execution starts, and does not change until it ends
 *
 *      If the relation is ordered, "the first rows" follows its order.
 *      Otherwise, which rows are taken is not specified, and only the number of rows is defined.
 *      Each operator defines which relation (the whole relation or each group) is sliced, and its order.
 *
 *      This is almost equivalent to `OFFSET start ROWS FETCH FIRST count ROWS ONLY` in SQL:2011,
 *      except that a `NULL` value is treated as same as empty, instead of raising an error.
 */
class row_slice {
public:

    /// @brief row position type.
    using position_type = constant_value<std::size_t>;

    /// @brief row count type.
    using size_type = constant_value<std::size_t>;

    /**
     * @brief constructs a new instance, which indicates the whole relation.
     */
    row_slice() noexcept;

    /**
     * @brief constructs a new instance.
     * @details To specify only the row count, write `{ {}, count }`.
     *      A single value like `{ count }` is not accepted, to avoid mistaking it for the start row position.
     * @param start the start row position, or empty to indicate the start from the first row
     * @param count the row count, or empty to indicate all rows
     */
    row_slice(
            position_type start,
            size_type count) noexcept;

    /**
     * @brief returns whether this slice indicates part of relation.
     * @details This only considers which rows are taken by this slice.
     *      For example, `{ 0, {} }` takes all rows of the relation, so that this returns false.
     *      Note that, operator==() distinguishes it from `{}`.
     * @return true if this slice may exclude some rows from the relation
     * @return false if this slice takes the whole relation,
     *      that is, start() is empty or immediate `0`, and count() is empty
     */
    [[nodiscard]] explicit operator bool() const noexcept;

    /**
     * @brief returns the start row position.
     * @return the start row position
     * @return empty if starts from the first row
     */
    [[nodiscard]] position_type& start() noexcept;

    /// @copydoc start()
    [[nodiscard]] const position_type& start() const noexcept;

    /**
     * @brief returns the row count to take.
     * @return the row count
     * @return empty if includes all rows
     */
    [[nodiscard]] size_type& count() noexcept;

    /// @copydoc count()
    [[nodiscard]] const size_type& count() const noexcept;

    /**
     * @brief returns whether the two row slicing are equivalent.
     * @details This compares the structure of slices, that is, `{ 0, {} }` and `{}` are different
     *      even though both of them take the whole relation.
     * @param lhs the first value
     * @param rhs the second value
     * @return true if lhs == rhs
     * @return false otherwise
     */
    friend bool operator==(row_slice const& lhs, row_slice const& rhs) noexcept;

    /**
     * @brief returns whether the two row slicing are not equivalent.
     * @param lhs the first value
     * @param rhs the second value
     * @return true if lhs != rhs
     * @return false otherwise
     */
    friend bool operator!=(row_slice const& lhs, row_slice const& rhs) noexcept;

    /**
     * @brief appends string representation of the given row slice.
     * @param out the target output
     * @param slice the target row slice
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, row_slice const& slice);

private:
    position_type start_;
    size_type count_;
};

} // namespace takatori::relation::details
