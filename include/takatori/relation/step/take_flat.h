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
 * @brief obtains first order rows from an upstream exchange (may be forward exchange).
 */
class take_flat : public expression {
public:
    /// @brief column mapping between the upstream exchange and the output relation.
    using column = details::mapping_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::take_flat;

    /**
     * @brief creates a new instance.
     * @param source the source relation, must be refer the upstream exchange
     * @param columns the column mappings between the upstream exchange and output relation
     * @param creator the object creator for internal elements
     */
    explicit take_flat(
            descriptor::relation source,
            std::vector<column, util::object_allocator<column>> columns,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the source relation, must be refer the source exchange
     * @param columns the column mappings between the upstream exchange and output relation
     */
    explicit take_flat(
            descriptor::relation source,
            std::initializer_list<column> columns = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit take_flat(take_flat const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit take_flat(take_flat&& other, util::object_creator creator);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] take_flat* clone(util::object_creator creator) const& override;
    [[nodiscard]] take_flat* clone(util::object_creator creator) && override;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the source relation, must be refer the source exchange.
     * @return the source relation
     */
    [[nodiscard]] descriptor::relation& source() noexcept;

    /// @copydoc source()
    [[nodiscard]] descriptor::relation const& source() const noexcept;

    /**
     * @brief returns the column mappings, from the upstream exchange to output relation.
     * @return the column mappings
     */
    [[nodiscard]] std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(take_flat const& a, take_flat const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(take_flat const& a, take_flat const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, take_flat const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    descriptor::relation source_;
    std::vector<column, util::object_allocator<column>> columns_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for take_flat.
 */
template<> struct type_of<step::take_flat::tag> : util::meta_type<step::take_flat> {};

} // namespace takatori::relation
