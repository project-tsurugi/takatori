#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "../expression.h"
#include "../expression_kind.h"

#include "../details/mapping_element.h"

#include <takatori/descriptor/relation.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

namespace takatori::relation::step {

/**
 * @brief obtains second order rows from an upstream exchange (may be shuffle exchange).
 */
class take_group : public expression {
public:
    /// @brief column mapping between the upstream exchange and the output relation.
    using column = details::mapping_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::take_group;

    /**
     * @brief creates a new instance.
     * @param source the source relation, must be refer the upstream exchange
     * @param columns the column mappings between the upstream exchange and output relation
     */
    explicit take_group(
            descriptor::relation source,
            std::vector<column> columns) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the source relation, must be refer the source exchange
     * @param columns the column mappings between the upstream exchange and output relation
     */
    explicit take_group(
            descriptor::relation source,
            std::initializer_list<column> columns = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit take_group(util::clone_tag_t, take_group const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit take_group(util::clone_tag_t, take_group&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] take_group* clone() const& override;
    [[nodiscard]] take_group* clone() && override;

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
     * @brief sets the source exchange.
     * @param source the source exchange
     * @return this
     */
    take_group& source(descriptor::relation source) noexcept;

    /**
     * @brief returns the column mappings, from the upstream exchange to output relation.
     * @return the column mappings
     */
    [[nodiscard]] std::vector<column>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column> const& columns() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(take_group const& a, take_group const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(take_group const& a, take_group const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, take_group const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    descriptor::relation source_;
    std::vector<column> columns_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for take_group.
 */
template<> struct type_of<step::take_group::tag> : util::meta_type<step::take_group> {};

} // namespace takatori::relation
