#pragma once

#include <initializer_list>
#include <vector>

#include "../expression.h"
#include "../expression_kind.h"

#include "../details/cogroup_element.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::relation::step {

/**
 * @brief obtains second order rows from two or more upstream exchanges and join them (the all must be shuffle exchanges).
 */
class take_cogroup : public expression {
public:
    /// @brief the individual group type.
    using group = details::cogroup_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::take_cogroup;

    /**
     * @brief creates a new instance.
     * @param groups the individual source groups information, from corresponded upstream exchanges
     * @param creator the object creator for internal elements
     */
    explicit take_cogroup(
            std::vector<group, util::object_allocator<group>> groups,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new instance.
     * @param groups the individual source groups information, from corresponded upstream exchanges
     */
    take_cogroup(std::initializer_list<group> groups);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit take_cogroup(take_cogroup const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit take_cogroup(take_cogroup&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    take_cogroup* clone(util::object_creator creator) const& override;
    take_cogroup* clone(util::object_creator creator) && override;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    output_port_type& output() noexcept;

    /// @copydoc output()
    output_port_type const& output() const noexcept;

    /**
     * @brief returns the individual source groups information.
     * Each group member must describes about the corresponded upstream exchange.
     * @return the source groups
     */
    std::vector<group, util::object_allocator<group>>& groups() noexcept;

    /// @copydoc groups()
    std::vector<group, util::object_allocator<group>> const& groups() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(take_cogroup const& a, take_cogroup const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(take_cogroup const& a, take_cogroup const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, take_cogroup const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    std::vector<group, util::object_allocator<group>> groups_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for take_cogroup.
 */
template<> struct type_of<step::take_cogroup::tag> : util::meta_type<step::take_cogroup> {};

} // namespace takatori::relation
