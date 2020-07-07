#pragma once

#include <ostream>

#include <takatori/descriptor/variable.h>
#include <takatori/scalar/expression.h>
#include <takatori/tree/tree_fragment_vector.h>
#include <takatori/util/rvalue_reference_wrapper.h>

#include "expression.h"
#include "expression_kind.h"
#include "details/values_row.h"

namespace takatori::relation {

/**
 * @brief constructs rows from scalar expressions.
 */
class values final : public expression {
public:
    /// @brief table column descriptor.
    using column = descriptor::variable;

    /// @brief row of each column value.
    using row = details::values_row;

    /// @brief the kind of this relation.
    static constexpr inline expression_kind tag = expression_kind::values;

    /**
     * @brief creates a new instance.
     * @param columns the destination columns
     * @param rows the each row data; must be ordered by `columns`
     * @param creator the object creator
     */
    explicit values(
            std::vector<column, util::object_allocator<column>> columns,
            std::vector<row, util::object_allocator<row>> rows,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new instance.
     * @param columns the destination columns
     * @param rows the each row data; must be ordered by `columns`
     * @attention this will take copy of arguments
     */
    explicit values(
            std::initializer_list<column> columns,
            std::initializer_list<std::initializer_list<util::rvalue_reference_wrapper<scalar::expression>>> rows);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit values(values const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit values(values&& other, util::object_creator creator);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] values* clone(util::object_creator creator) const& override;
    [[nodiscard]] values* clone(util::object_creator creator) && override;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the columns on the destination table.
     * @return This designates each mapping between table column and its value to values.
     *      Each elements in the individual rows() must be ordered by this sequence of columns.
     */
    [[nodiscard]] std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns the rows to values.
     * @details The individual elements in each row must be ordered by columns().
     * @return the value rows
     */
    [[nodiscard]] tree::tree_fragment_vector<row>& rows() noexcept;

    /// @copydoc rows()
    [[nodiscard]] tree::tree_fragment_vector<row> const& rows() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(values const& a, values const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(values const& a, values const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, values const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    std::vector<column, util::object_allocator<column>> columns_;
    tree::tree_fragment_vector<row> rows_;
};

/**
 * @brief type_of for values.
 */
template<> struct type_of<values::tag> : util::meta_type<values> {};

} // namespace takatori::relation
