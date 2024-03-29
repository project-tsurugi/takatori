#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "../expression.h"
#include "../expression_kind.h"

#include <takatori/descriptor/variable.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

namespace takatori::relation::intermediate {

/**
 * @brief removes duplicated rows in the input relation.
 */
class distinct : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::distinct_relation;

    /**
     * @brief creates a new object.
     * @param group_keys the key columns on the input relation:
     *      this operation distinguishes individual rows by these keys
     */
    explicit distinct(
            std::vector<descriptor::variable> group_keys) noexcept;

    /**
     * @brief creates a new object.
     * @param group_keys the key columns on the input relation:
     *      this operation distinguishes individual rows by these keys
     */
    distinct(std::initializer_list<descriptor::variable> group_keys);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit distinct(util::clone_tag_t, distinct const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit distinct(util::clone_tag_t, distinct&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] distinct* clone() const& override;
    [[nodiscard]] distinct* clone() && override;

    /**
     * @brief returns the input port.
     * @return the input port
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
     * @brief returns the distinct key columns on the input relation.
     * @return the distinct keys
     */
    [[nodiscard]] std::vector<descriptor::variable>& group_keys() noexcept;

    /// @copydoc group_keys()
    [[nodiscard]] std::vector<descriptor::variable> const& group_keys() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(distinct const& a, distinct const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(distinct const& a, distinct const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, distinct const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    std::vector<descriptor::variable> group_keys_;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for distinct.
 */
template<> struct type_of<intermediate::distinct::tag> : util::meta_type<intermediate::distinct> {};

} // namespace takatori::relation

