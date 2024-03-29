#pragma once

#include <initializer_list>
#include <memory>
#include <optional>
#include <vector>

#include "expression.h"
#include "expression_kind.h"

#include "details/search_key_element.h"
#include "details/mapping_element.h"
#include "details/range_endpoint.h"

#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/variable.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

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
     * @param columns the target columns to scan.
     *      Each `source` must be a column on the `source` relation,
     *      and `destination` represents a new column of the resulting relation
     * @param lower the lower end-point specification.
     *      The whole keys must be equivalent to or prefix of the key of the `source` relation,
     *      and each `value` cannot include any variables declared in `columns.destination`s 
     * @param upper the upper end-point specification.
     *      The whole keys must be equivalent to or prefix of the key of the `source` relation,
     *      and each `value` cannot include any variables declared in `columns.destination`s 
     * @param limit the maximum number of output records
     */
    explicit scan(
            descriptor::relation source,
            std::vector<column> columns,
            endpoint lower,
            endpoint upper,
            std::optional<std::size_t> limit) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the source relation
     * @param columns the target columns to scan.
     *      Each `source` must be a column on the `source` relation,
     *      and `destination` represents a new column of the resulting relation
     * @param lower the lower end-point specification.
     *      The whole keys must be equivalent to or prefix of the key of the `source` relation,
     *      and each `value` cannot include any variables declared in `columns.destination`s 
     * @param upper the upper end-point specification.
     *      The whole keys must be equivalent to or prefix of the key of the `source` relation,
     *      and each `value` cannot include any variables declared in `columns.destination`s 
     * @param limit the maximum number of output records
     * @attention this may take copies of arguments
     */
    explicit scan(
            descriptor::relation source,
            std::initializer_list<column> columns,
            endpoint lower = {},
            endpoint upper = {},
            std::optional<std::size_t> limit = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit scan(util::clone_tag_t, scan const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit scan(util::clone_tag_t, scan&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] scan* clone() const& override;
    [[nodiscard]] scan* clone() && override;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the source relation.
     * @return the source relation
     */
    [[nodiscard]] descriptor::relation& source() noexcept;

    /// @copydoc source()
    [[nodiscard]] descriptor::relation const& source() const noexcept;

    /**
     * @brief returns the target columns to scan.
     * @return the target columns
     */
    [[nodiscard]] std::vector<column>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column> const& columns() const noexcept;

    /**
     * @brief returns the lower end-point specification.
     * @return the lower end-point specification
     */
    [[nodiscard]] endpoint& lower() noexcept;

    /// @copydoc lower()
    [[nodiscard]] endpoint const& lower() const noexcept;

    /**
     * @brief returns the upper end-point specification.
     * @return the upper end-point specification
     */
    [[nodiscard]] endpoint& upper() noexcept;

    /// @copydoc upper()
    [[nodiscard]] endpoint const& upper() const noexcept;

    /**
     * @brief returns the maximum number of output records.
     * @return the maximum number of output records
     * @return empty if it is unlimited
     */
    [[nodiscard]] std::optional<std::size_t> const& limit() const noexcept;

    /**
     * @brief sets the maximum number of output records.
     * @param limit the maximum number
     * @return this
     */
    scan& limit(std::optional<std::size_t> limit) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(scan const& a, scan const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
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
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    descriptor::relation source_;
    std::vector<column> columns_;
    endpoint lower_;
    endpoint upper_;
    std::optional<std::size_t> limit_;
};

/**
 * @brief type_of for scan.
 */
template<> struct type_of<scan::tag> : util::meta_type<scan> {};

} // namespace takatori::relation
