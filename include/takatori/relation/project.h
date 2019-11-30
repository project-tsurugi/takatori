#pragma once

#include <initializer_list>
#include <vector>

#include "expression.h"
#include "expression_kind.h"

#include "takatori/scalar/details/variable_declarator.h"

#include "takatori/tree/tree_fragment_vector.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"
#include "takatori/util/rvalue_reference_wrapper.h"

namespace takatori::relation {

/**
 * @brief adds columns into relation.
 */
class project : public expression {
public:
    /// @brief the variable declarator type.
    using column = scalar::details::variable_declarator<project>;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::project;

    /**
     * @brief creates a new object.
     * @param columns the columns definition to add to the input relation
     * @param creator the object creator for internal elements
     */
    explicit project(
            std::vector<column, util::object_allocator<column>> columns,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the columns definition to add to the input relation
     * @warning this may takes a copy of the column definitions
     */
    explicit project(std::initializer_list<util::rvalue_reference_wrapper<column>> columns); // NOLINT

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit project(project const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit project(project&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    project* clone(util::object_creator creator) const& override;
    project* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * @return the input port
     */
    input_port_type& input() noexcept;

    /// @copydoc input()
    input_port_type const& input() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    output_port_type& output() noexcept;

    /// @copydoc output()
    output_port_type const& output() const noexcept;

    /**
     * @brief returns the columns definition.
     * @return the columns definition
     */
    tree::tree_fragment_vector<column>& columns() noexcept;

    /// @copydoc columns()
    tree::tree_fragment_vector<column> const& columns() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(project const& a, project const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(project const& a, project const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, project const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    tree::tree_fragment_vector<column> columns_;
};

/**
 * @brief type_of for select.
 */
template<> struct type_of<project::tag> : util::meta_type<project> {};

} // namespace takatori::relation
