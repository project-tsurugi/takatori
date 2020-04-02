#pragma once

#include <initializer_list>
#include <vector>

#include "../expression.h"
#include "../expression_kind.h"

#include "../details/mapping_element.h"

#include <takatori/descriptor/relation.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation::step {

/**
 * @brief puts process output to the downstream exchange.
 */
class offer : public expression {
public:
    /// @brief column mapping between the input relation and downstream exchange.
    using column = details::mapping_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::offer;

    /**
     * @brief creates a new instance.
     * @param destination the destination relation, must be refer the downstream exchange
     * @param columns the column mappings between the input relation and downstream exchange;
     *      This may or may not cover all the columns on the target relation.
     *      The unspecified columns are treated as `unknown`
     * @param creator the object creator for internal elements
     */
    explicit offer(
            descriptor::relation destination,
            std::vector<column, util::object_allocator<column>> columns,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new instance.
     * @param destination the destination relation, must be refer the destination exchange
     * @param columns the column mappings between the input relation and downstream exchange;
     *      This may or may not cover all the columns on the target relation.
     *      The unspecified columns are treated as `unknown`
     */
    explicit offer(
            descriptor::relation destination,
            std::initializer_list<column> columns);

    /**
     * @brief creates a new object.
     * @param other the copy destination
     * @param creator the object creator
     */
    explicit offer(offer const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move destination
     * @param creator the object creator
     */
    explicit offer(offer&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    offer* clone(util::object_creator creator) const& override;
    offer* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * @attention The input must be a first order relation.
     * @return the input port
     * @see take_flat
     */
    input_port_type& input() noexcept;

    /// @copydoc input()
    input_port_type const& input() const noexcept;

    /**
     * @brief returns the destination relation, must be refer the destination exchange.
     * @return the destination relation
     */
    descriptor::relation const& destination() const noexcept;

    /**
     * @brief sets the destination exchange.
     * @param destination the destination exchange
     * @return this
     */
    offer& destination(descriptor::relation destination) noexcept;

    /**
     * @brief returns the column mappings, from the input relation to downstream exchange.
     * @return the column mappings
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
    friend bool operator==(offer const& a, offer const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(offer const& a, offer const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, offer const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    descriptor::relation destination_;
    std::vector<column, util::object_allocator<column>> columns_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for offer.
 */
template<> struct type_of<step::offer::tag> : util::meta_type<step::offer> {};

} // namespace takatori::relation
