#pragma once

#include <initializer_list>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "join_kind.h"

#include "details/search_key_element.h"
#include "details/mapping_element.h"
#include "details/range_endpoint.h"

#include "takatori/descriptor/relation.h"
#include "takatori/descriptor/variable.h"

#include "takatori/scalar/expression.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"
#include "takatori/util/rvalue_ptr.h"

namespace takatori::relation {

/**
 * @brief join input with rows in storage.
 */
class join_scan : public expression {
public:
    /// @brief the join direction
    using operator_kind_type = relation::join_kind;

    /// @brief the scan column type.
    using column = details::mapping_element;

    /// @brief the key piece type.
    using key = details::search_key_element<join_scan>;

    /// @brief the end-point specification
    using endpoint = details::range_endpoint<join_scan, key>;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::join_scan;

    /**
     * @brief creates a new instance.
     * @param operator_kind the join kind
     * @param source the external source relation, which represents right build input of join operation
     * @param columns the columns to be joined, on the source relation
     * @param lower the lower end-point specification
     * @param upper the upper end-point specification
     * @param condition the extra join condition expression
     * @param creator the object creator for internal elements
     */
    explicit join_scan(
            operator_kind_type operator_kind,
            descriptor::relation source,
            std::vector<column, util::object_allocator<column>> columns,
            endpoint lower,
            endpoint upper,
            util::unique_object_ptr<scalar::expression> condition,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the join kind
     * @param source the external source relation, which represents right build input of join operation
     * @param columns the columns to be joined, on the source relation
     * @param lower the lower end-point specification
     * @param upper the upper end-point specification
     * @param condition the extra join condition expression
     */
    explicit join_scan(
            operator_kind_type operator_kind,
            descriptor::relation source,
            std::initializer_list<column> columns,
            endpoint lower,
            endpoint upper,
            util::rvalue_ptr<scalar::expression> condition = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit join_scan(join_scan const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit join_scan(join_scan&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    join_scan* clone(util::object_creator creator) const& override;
    join_scan* clone(util::object_creator creator) && override;

    /**
     * @brief returns the left prove input port.
     * @return the left prove input port
     */
    input_port_type& left() noexcept;

    /// @copydoc left()
    input_port_type const& left() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    output_port_type& output() noexcept;

    /// @copydoc output()
    output_port_type const& output() const noexcept;

    /**
     * @brief returns the join kind.
     * @return the join kind
     */
    operator_kind_type operator_kind() const noexcept;

    /**
     * @brief sets the join kind.
     * @param operator_kind the join kind
     * @return this
     */
    join_scan& operator_kind(operator_kind_type operator_kind) noexcept;

    /**
     * @brief returns the external source relation (as build input).
     * @return the source relation
     */
    descriptor::relation const& source() const noexcept;

    /**
     * @brief sets the external source relation.
     * @param source the external source relation
     * @return this
     */
    virtual join_scan& source(descriptor::relation source) noexcept;

    /**
     * @brief returns the target columns to scan.
     * @return the target columns
     */
    std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns the lower end-point specification.
     * @return the lower end-point specification
     */
    endpoint& lower() noexcept;

    /// @copydoc lower()
    endpoint const& lower() const noexcept;

    /**
     * @brief returns the upper end-point specification.
     * @return the upper end-point specification
     */
    endpoint& upper() noexcept;

    /// @copydoc upper()
    endpoint const& upper() const noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::optional_ptr<scalar::expression> condition() noexcept;

    /// @copydoc condition()
    util::optional_ptr<scalar::expression const> condition() const noexcept;

    /**
     * @brief releases the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    util::unique_object_ptr<scalar::expression> release_condition() noexcept;

    /**
     * @brief sets the condition expression.
     * @param condition the replacement
     * @return this
     */
    join_scan& condition(util::unique_object_ptr<scalar::expression> condition) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(join_scan const& a, join_scan const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(join_scan const& a, join_scan const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, join_scan const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type left_;
    output_port_type output_;
    operator_kind_type operator_kind_;
    descriptor::relation source_;
    std::vector<column, util::object_allocator<column>> columns_;
    endpoint lower_;
    endpoint upper_;
    util::unique_object_ptr<scalar::expression> condition_;
};

/**
 * @brief type_of for join_scan_point.
 */
template<> struct type_of<join_scan::tag> : util::meta_type<join_scan> {};

} // namespace takatori::relation
