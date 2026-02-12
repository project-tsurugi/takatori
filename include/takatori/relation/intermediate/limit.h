#pragma once

#include <vector>

#include "../expression.h"
#include "../expression_kind.h"
#include "../details/sort_key_element.h"
#include "../details/row_slice.h"

#include <takatori/descriptor/variable.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

namespace takatori::relation::intermediate {

/**
 * @brief limits the number of rows in the input relation or its partitions.
 */
class limit : public expression {
public:
    /// @brief sort key type.
    using sort_key = details::sort_key_element;

    /// @brief row slice type.
    using row_slice_type = details::row_slice;

    /// @brief the kind of this expression.
    static constexpr expression_kind tag = expression_kind::limit_relation;

    /**
     * @brief creates a new object.
     * @param group_keys the group key columns on the input relation, or empty to limit for whole relation
     * @param sort_keys the sort key: this operation will only keep the rows from head of the sorted relation
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of the relation
     */
    explicit limit(
            std::vector<descriptor::variable> group_keys = {},
            std::vector<sort_key> sort_keys = {},
            row_slice_type row_slice = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param row_slice the row slice: this operation will only keep the rows in the specified range of the relation
     */
    explicit limit(row_slice_type row_slice) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit limit(util::clone_tag_t, limit const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit limit(util::clone_tag_t, limit&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] limit* clone() const& override;
    [[nodiscard]] limit* clone() && override;

    /**
     * @brief returns the input port.
     * @return the input port
     */
    [[nodiscard]] input_port_type& input() noexcept;

    /// @copydoc input()
    [[nodiscard]] input_port_type const& input() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the limit key columns on the input relation.
     * @return the limit keys
     */
    [[nodiscard]] std::vector<descriptor::variable>& group_keys() noexcept;

    /// @copydoc group_keys()
    [[nodiscard]] std::vector<descriptor::variable> const& group_keys() const noexcept;

    /**
     * @brief returns the sort key.
     * @return the sort key
     */
    [[nodiscard]] std::vector<sort_key>& sort_keys() noexcept;

    /// @brief sort_keys()
    [[nodiscard]] std::vector<sort_key> const& sort_keys() const noexcept;

    /**
     * @brief returns the row slice.
     * @details If the sort key is specified, this exchange will sort rows in each group by using it.
     *      Each sort key column must also appear in the columns(), but not appear in group_keys().
     *      If multiple sort key elements are specified, the earlier elements have higher priority.
     *      If there are out of slice rows in each group, they will be discarded after sorting.
     * @return the row slice
     */
    [[nodiscard]] row_slice_type& row_slice() noexcept;

    /// @copydoc row_slice()
    [[nodiscard]] row_slice_type const& row_slice() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(limit const& a, limit const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(limit const& a, limit const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, limit const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    std::vector<descriptor::variable> group_keys_;
    std::vector<sort_key> sort_keys_;
    row_slice_type row_slice_;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for limit.
 */
template<> struct type_of<intermediate::limit::tag> : util::meta_type<intermediate::limit> {};

} // namespace takatori::relation

