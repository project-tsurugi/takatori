#pragma once

#include <cstddef>

#include <initializer_list>
#include <vector>

#include "../expression.h"
#include "../expression_kind.h"

#include <takatori/descriptor/variable.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation::intermediate {

/**
 * @brief limits the number of rows in the input relation or its partitions.
 */
class limit : public expression {
public:
    /// @brief type of the number of rows.
    using size_type = std::size_t;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::limit_relation;

    /**
     * @brief creates a new object.
     * @param count the number of rows to keep in the input relation or each partition
     * @param keys the partitioning key columns on the input relation, or empty to limit for whole relation
     * @param creator the object creator for internal elements
     */
    explicit limit(
            size_type count,
            std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> keys,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param count the number of rows to keep in the input relation
     * @param keys the grouping key columns on the input relation, or empty to limit for whole relation
     */
    explicit limit(
            size_type count,
            std::initializer_list<descriptor::variable> keys = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit limit(limit const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
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
     * @return the input port
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
     * @brief returns the limit key columns on the input relation.
     * @return the limit keys
     */
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& keys() noexcept;

    /// @copydoc keys()
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& keys() const noexcept;

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
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> keys_;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for limit.
 */
template<> struct type_of<intermediate::limit::tag> : util::meta_type<intermediate::limit> {};

} // namespace takatori::relation

