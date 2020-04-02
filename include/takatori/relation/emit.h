#pragma once

#include <initializer_list>
#include <optional>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "details/emit_element.h"
#include "details/sort_key_element.h"

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation {

/**
 * @brief emits rows in the input relation.
 */
class emit : public expression {
public:
    /// @brief type of the number of rows.
    using size_type = std::size_t;

    /// @brief emit column type.
    using column = details::emit_element;

    /// @brief sort key type.
    using sort_key = details::sort_key_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::emit;

    /**
     * @brief creates a new object.
     * @param columns the columns to be emitted in the input relation
     * @param sort_keys the sort key which represents how sort the emitting rows
     * @param creator the object creator for internal elements
     * @param limit the maximum number of rows to emit
     * @attention the sort key and row number limit are only available in intermediate execution plan;
     *      it will be omitted in the step execution plan
     */
    explicit emit(
            std::vector<column, util::object_allocator<column>> columns,
            std::vector<sort_key, util::object_allocator<sort_key>> sort_keys,
            std::optional<size_type> limit = {},
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the columns to be emitted in the input relation
     * @param sort_keys the key to sort the emitting rows
     * @param limit the maximum number of rows to emit
     * @attention compiler may confuse when both the column name and sort direction were omitted.
     * for example, the following code snippet constructs an expression with 2 columns and 0 key elements:
     * @code {.cpp}
     * emit expr {
     *     {
     *         var(1),
     *     },
     *     {
     *         var(2),
     *     },
     * }
     * @endcode
     * To avoid the above problem, please specify `emit::key { ... }` to key elements or
     * explicitly pass `sort_direction::ascendant`, like as following:
     * @code {.cpp}
     * emit expr {
     *     {
     *         var(1),
     *     },
     *     {
     *         emit::sort_key { var(2) },
     *         { var(3), sort_direction::ascendant, },
     *     },
     * }
     * @endcode
     * Finally, the above snippet achieves to constructs an expresion with 1 column and 2 key elements.
     * @attention the sort key and row number limit are only available in intermediate execution plan;
     *      it will be omitted in the step execution plan
     * @attention this may take copies of the given arguments
     */
    emit(
            std::initializer_list<column> columns,
            std::initializer_list<sort_key> sort_keys = {},
            std::optional<size_type> limit = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit emit(emit const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit emit(emit&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    emit* clone(util::object_creator creator) const& override;
    emit* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * @return the input port
     */
    input_port_type& input() noexcept;

    /// @copydoc input()
    input_port_type const& input() const noexcept;

    /**
     * @brief returns emit target columns in the input relation.
     * @return the emit target columns
     */
    std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns the sort key, which represents how sort the emitting rows.
     * @return the sort key
     * @attention this is only available in intermediate execution plan;
     *      it will be omitted in the step execution plan
     */
    std::vector<sort_key, util::object_allocator<sort_key>>& sort_keys() noexcept;

    /// @brief sort_keys()
    std::vector<sort_key, util::object_allocator<sort_key>> const& sort_keys() const noexcept;

    /**
     * @brief returns the max number of exchange rows.
     * @return the number of exchange rows limit
     * @return empty if it is unlimited
     */
    std::optional<size_type> const& limit() const noexcept;

    /**
     * @brief sets the max number of exchange rows.
     * @param limit the number of exchange rows limit
     * @return this
     */
    emit& limit(std::optional<size_type> limit) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(emit const& a, emit const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(emit const& a, emit const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, emit const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    std::vector<column, util::object_allocator<column>> columns_;
    std::vector<sort_key, util::object_allocator<sort_key>> sort_keys_;
    std::optional<size_type> limit_ {};
};

/**
 * @brief type_of for where.
 */
template<> struct type_of<emit::tag> : util::meta_type<emit> {};

} // namespace takatori::relation
