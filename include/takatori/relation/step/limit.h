#pragma once

#include <cstddef>

#include "../expression.h"
#include "../expression_kind.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::relation::step {

/**
 * @brief limits the number of rows in the each input group.
 */
class limit : public expression {
public:
    /// @brief type of the number of rows.
    using size_type = std::size_t;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::limit_group;

    /**
     * @brief creates a new instance.
     * @param count the number of rows to keep in the each input group
     * @param creator the object creator for internal elements
     */
    explicit limit(
            size_type count,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy destination
     * @param creator the object creator
     */
    explicit limit(limit const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move destination
     * @param creator the object creator
     */
    explicit limit(limit&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    limit* clone(util::object_creator creator) const& override;
    limit* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * @attention The input must be a group relation.
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
     * @brief returns the maximum number of rows.
     * @return the limit size
     */
    size_type count() const noexcept;

    /**
     * @brief sets the maximum number of rows.
     * @param count the limit size
     * @return this
     */
    limit& count(size_type count) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(limit const& a, limit const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(limit const& a, limit const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, limit const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    size_type count_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for limit.
 */
template<> struct type_of<step::limit::tag> : util::meta_type<step::limit> {};

} // namespace takatori::relation
