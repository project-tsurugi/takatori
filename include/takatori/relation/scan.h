#pragma once

#include <initializer_list>
#include <vector>

#include "expression.h"
#include "expression_kind.h"

#include "details/search_key_element.h"
#include "details/mapping_element.h"
#include "details/range_endpoint.h"

#include "takatori/descriptor/relation.h"
#include "takatori/descriptor/variable.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::relation {

/**
 * @brief obtains rows an external relation by their key range.
 */
class scan : public expression {
public:
    /// @brief the scan column type.
    using column = details::mapping_element;

    /// @brief the key piece type.
    using key = details::search_key_element<scan>;

    /// @brief the end-point type.
    using endpoint = details::range_endpoint<scan, key>;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::scan;

    /**
     * @brief creates a new instance.
     * @param source the source relation
     * @param columns the target columns to scan
     * @param lower the lower end-point specification
     * @param upper the upper end-point specification
     * @param creator the object creator for internal elements
     */
    explicit scan(
            descriptor::relation source,
            std::vector<column, util::object_allocator<column>> columns,
            endpoint lower,
            endpoint upper,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the source relation
     * @param columns the target columns to scan
     * @param lower the lower end-point specification
     * @param upper the upper end-point specification
     * @attention this may take copies of arguments
     */
    explicit scan(
            descriptor::relation source,
            std::initializer_list<util::rvalue_reference_wrapper<column>> columns,
            endpoint lower = {},
            endpoint upper = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit scan(scan const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit scan(scan&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    scan* clone(util::object_creator creator) const& override;
    scan* clone(util::object_creator creator) && override;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    output_port_type& output() noexcept;

    /// @copydoc output()
    output_port_type const& output() const noexcept;

    /**
     * @brief returns the source relation.
     * @return the source relation
     */
    descriptor::relation const& source() const noexcept;

    /**
     * @brief sets the source relation.
     * @param source the source relation
     * @return this
     */
    scan& source(descriptor::relation source) noexcept;

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
     * @brief returns whether or not the two elements are equivalent.
     * This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(scan const& a, scan const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(scan const& a, scan const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, scan const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    descriptor::relation source_;
    std::vector<column, util::object_allocator<column>> columns_;
    endpoint lower_;
    endpoint upper_;
};

/**
 * @brief type_of for scan_range.
 */
template<> struct type_of<scan::tag> : util::meta_type<scan> {};

} // namespace takatori::relation
