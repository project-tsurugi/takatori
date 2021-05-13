#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "expression.h"
#include "expression_kind.h"

#include <takatori/scalar/details/variable_declarator.h>

#include <takatori/tree/tree_fragment_vector.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>
#include <takatori/util/rvalue_reference_wrapper.h>

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
     */
    explicit project(
            std::vector<column> columns) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the columns definition to add to the input relation
     * @warning this may takes a copy of the column definitions
     */
    explicit project(std::initializer_list<util::rvalue_reference_wrapper<column>> columns); // NOLINT

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit project(util::clone_tag_t, project const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit project(util::clone_tag_t, project&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] project* clone() const& override;
    [[nodiscard]] project* clone() && override;

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
     * @brief returns the columns definition.
     * @return the columns definition
     */
    [[nodiscard]] tree::tree_fragment_vector<column>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] tree::tree_fragment_vector<column> const& columns() const noexcept;

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
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    tree::tree_fragment_vector<column> columns_;
};

/**
 * @brief type_of for project.
 */
template<> struct type_of<project::tag> : util::meta_type<project> {};

} // namespace takatori::relation
