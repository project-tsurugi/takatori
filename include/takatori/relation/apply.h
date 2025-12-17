#pragma once

#include <initializer_list>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "apply_kind.h"

#include <takatori/descriptor/function.h>
#include <takatori/descriptor/variable.h>

#include <takatori/tree/tree_element_vector.h>

#include <takatori/scalar/expression.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>
#include <takatori/util/reference_vector.h>
#include <takatori/util/rvalue_initializer_list.h>

namespace takatori::relation {

/**
 * @brief represents a table-valued function application expression.
 */
class apply final : public expression {
public:
    /// @brief the kind of this relation.
    static constexpr expression_kind tag = expression_kind::apply;

    /// @brief the apply operator kind type.
    using operator_kind_type = apply_kind;

    /**
     * @brief creates a new instance.
     * @param operator_kind the apply operator kind
     * @param function the descriptor of target function, must be a table-valued
     * @param arguments the function arguments
     * @param columns the output columns definition.
     *      Each column position corresponds to the output columns of the function resulting table.
     */
    explicit apply(
            operator_kind_type operator_kind,
            descriptor::function function,
            util::reference_vector<scalar::expression> arguments,
            std::vector<descriptor::variable> columns) noexcept;

    /**
     * @brief creates a new instance.
     * @param function the descriptor of target function, must be a table-valued
     * @param arguments the function arguments
     * @param columns the output columns definition.
     *      Each column position corresponds to the output columns of the function resulting table.
     * @param operator_kind the apply operator kind
     * @attention this may take copies of given expressions
     */
    apply(
            descriptor::function function,
            util::rvalue_initializer_list<scalar::expression> arguments,
            std::initializer_list<descriptor::variable> columns,
            operator_kind_type operator_kind = operator_kind_type::cross);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit apply(util::clone_tag_t, apply const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit apply(util::clone_tag_t, apply&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] apply* clone() const& override;
    [[nodiscard]] apply* clone() && override;

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
     * @brief returns the kind of this apply expression.
     * @return the kind of this apply expression.
     */
    [[nodiscard]] operator_kind_type operator_kind() const noexcept;

    /**
     * @brief sets the operator kind of this apply expression.
     * @param kind the operator kind
     * @return the operator kind
     */
    apply& operator_kind(operator_kind_type kind) noexcept;

    /**
     * @brief returns the descriptor of target function.
     * @return the descriptor of target function.
     * @note the descriptor represents a table-valued function.
     */
    [[nodiscard]] descriptor::function& function() noexcept;

    /// @copydoc function()
    [[nodiscard]] descriptor::function const& function() const noexcept;

    /**
     * @brief returns the function arguments.
     * @return the function arguments
     */
    [[nodiscard]] tree::tree_element_vector<scalar::expression>& arguments() noexcept;

    /// @copydoc arguments()
    [[nodiscard]] tree::tree_element_vector<scalar::expression> const& arguments() const noexcept;

    /**
     * @brief returns the output columns definition.
     * @details each column position corresponds to the output columns of the function resulting table.
     * @return the output columns definition.
     */
    [[nodiscard]] std::vector<descriptor::variable>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<descriptor::variable> const& columns() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(apply const& a, apply const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(apply const& a, apply const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, apply const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    operator_kind_type operator_kind_;
    descriptor::function function_;
    tree::tree_element_vector<scalar::expression> arguments_;
    std::vector<descriptor::variable> columns_;
};

/**
 * @brief type_of for apply.
 */
template<> struct type_of<apply::tag> : util::meta_type<apply> {};


} // namespace takatori::relation

