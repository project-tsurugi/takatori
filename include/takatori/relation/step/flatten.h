#pragma once

#include "../expression.h"
#include "../expression_kind.h"

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

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
     */
    explicit flatten() noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy destination
     */
    explicit flatten(util::clone_tag_t, flatten const& other);

    /**
     * @brief creates a new object.
     * @param other the move destination
     */
    explicit flatten(util::clone_tag_t, flatten&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] flatten* clone() const& override;
    [[nodiscard]] flatten* clone() && override;

    /**
     * @brief returns the input port.
     * @attention The input must be a group relation.
     * @return the input port
     * @see take_group
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
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(flatten const& a, flatten const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
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
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
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
