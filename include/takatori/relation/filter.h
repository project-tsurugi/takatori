#pragma once

#include "expression.h"
#include "expression_kind.h"

#include <takatori/scalar/expression.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation {

/**
 * @brief removes rows from relation.
 */
class filter : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::filter;

    /**
     * @brief creates a new object.
     * @param condition the condition expression
     * @param creator the object creator for internal elements
     */
    explicit filter(
            util::unique_object_ptr<scalar::expression> condition,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param condition the condition expression
     * @attention this may take copies of the given expression
     */
    explicit filter(scalar::expression&& condition) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit filter(filter const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit filter(filter&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    filter* clone(util::object_creator creator) const& override;
    filter* clone(util::object_creator creator) && override;

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
     * @brief returns the condition expression.
     * @return the condition expression
     * @warning undefined behavior if the expression is absent
     */
    scalar::expression& condition() noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @warning undefined behavior if the expression is absent
     */
    scalar::expression const& condition() const noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::optional_ptr<scalar::expression> optional_condition() noexcept;

    /// @copydoc optional_condition()
    util::optional_ptr<scalar::expression const> optional_condition() const noexcept;

    /**
     * @brief releases the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::unique_object_ptr<scalar::expression> release_condition() noexcept;

    /**
     * @brief sets the condition expression.
     * @param condition the replacement
     * @return this
     */
    filter& condition(util::unique_object_ptr<scalar::expression> condition) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(filter const& a, filter const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(filter const& a, filter const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, filter const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    util::unique_object_ptr<scalar::expression> condition_;
};

/**
 * @brief type_of for where.
 */
template<> struct type_of<filter::tag> : util::meta_type<filter> {};

} // namespace takatori::relation
