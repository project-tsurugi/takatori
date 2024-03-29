#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "../expression.h"
#include "../expression_kind.h"

#include "../details/cogroup_element.h"

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

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
     */
    explicit take_cogroup(
            std::vector<group> groups) noexcept;

    /**
     * @brief creates a new instance.
     * @param groups the individual source groups information, from corresponded upstream exchanges
     */
    take_cogroup(std::initializer_list<group> groups);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit take_cogroup(util::clone_tag_t, take_cogroup const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit take_cogroup(util::clone_tag_t, take_cogroup&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] take_cogroup* clone() const& override;
    [[nodiscard]] take_cogroup* clone() && override;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the individual source groups information.
     * @details Each group member must describes about the corresponded upstream exchange.
     * @return the source groups
     */
    [[nodiscard]] std::vector<group>& groups() noexcept;

    /// @copydoc groups()
    [[nodiscard]] std::vector<group> const& groups() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(take_cogroup const& a, take_cogroup const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
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
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    std::vector<group> groups_;
};

} // namespace takatori::relation::step

namespace takatori::relation {

/**
 * @brief type_of for take_cogroup.
 */
template<> struct type_of<step::take_cogroup::tag> : util::meta_type<step::take_cogroup> {};

} // namespace takatori::relation
