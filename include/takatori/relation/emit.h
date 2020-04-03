#pragma once

#include <initializer_list>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "details/emit_element.h"

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation {

/**
 * @brief emits rows in the input relation.
 */
class emit : public expression {
public:
    /// @brief emit column type.
    using column = details::emit_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::emit;

    /**
     * @brief creates a new object.
     * @param columns the columns to be emitted in the input relation
     * @param creator the object creator for internal elements
     */
    explicit emit(
            std::vector<column, util::object_allocator<column>> columns,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the columns to be emitted in the input relation
     * @attention this may take copies of the given arguments
     */
    emit(std::initializer_list<column> columns);

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
};

/**
 * @brief type_of for where.
 */
template<> struct type_of<emit::tag> : util::meta_type<emit> {};

} // namespace takatori::relation
