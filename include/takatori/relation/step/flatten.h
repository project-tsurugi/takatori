#pragma once

#include "../expression.h"
#include "../expression_kind.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::relation::step {

/**
 * @brief flatten the second order relation from the input, and outputs as first order.
 */
class flatten : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::flatten_group;

    /**
     * @brief creates a new instance.
     * @param creator the object creator for internal elements
     */
    explicit flatten(util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy destination
     * @param creator the object creator
     */
    explicit flatten(flatten const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move destination
     * @param creator the object creator
     */
    explicit flatten(flatten&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    flatten* clone(util::object_creator creator) const& override;
    flatten* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * The input must be a group relation.
     * @return the input port
     * @see take_group
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
     * @brief returns whether or not the two elements are equivalent.
     * This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(flatten const& a, flatten const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(flatten const& a, flatten const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, flatten const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for flatten.
 */
template<> struct type_of<step::flatten::tag> : util::meta_type<step::flatten> {};

} // namespace takatori::relation
