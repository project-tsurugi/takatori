#pragma once

#include "../expression.h"
#include "../expression_kind.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::relation::step {

/**
 * @brief obtains an intersection of two input groups.
 * Actually this works that limits the number of rows in the first input group, up to the number of rows in second one.
 */
class intersection : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::intersection_group;

    /**
     * @brief creates a new instance.
     * @param creator the object creator for internal elements
     */
    explicit intersection(util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy destination
     * @param creator the object creator
     */
    explicit intersection(intersection const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move destination
     * @param creator the object creator
     */
    explicit intersection(intersection&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    intersection* clone(util::object_creator creator) const& override;
    intersection* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * The input must be a co-group relation consists of two groups.
     * @return the input port
     * @see take_cogroup
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
    friend bool operator==(intersection const& a, intersection const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(intersection const& a, intersection const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, intersection const& value);

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
 * @brief type_of for intersect.
 */
template<> struct type_of<step::intersection::tag> : util::meta_type<step::intersection> {};

} // namespace takatori::relation
