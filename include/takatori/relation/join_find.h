#pragma once

#include <initializer_list>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "join_kind.h"

#include "details/search_key_element.h"
#include "details/mapping_element.h"

#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/variable.h>

#include <takatori/tree/tree_fragment_vector.h>

#include <takatori/scalar/expression.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/ownership_reference.h>
#include <takatori/util/rvalue_ptr.h>
#include <takatori/util/rvalue_reference_wrapper.h>

namespace takatori::relation {

/**
 * @brief join input with rows in storage.
 */
class join_find : public expression {
public:
    /// @brief the join direction
    using operator_kind_type = relation::join_kind;

    /// @brief the scan column type.
    using column = details::mapping_element;

    /// @brief the key piece type.
    using key = details::search_key_element<join_find>;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::join_find;

    /**
     * @brief creates a new instance.
     * @param operator_kind the join kind
     * @param source the external source relation, which represents right build input of join operation
     * @param columns the columns to be joined, on the source relation.
     *      Each `source` must be a column on the `source` relation,
     *      and `destination` represents a new column of the resulting relation
     * @param keys the join key pieces.
     *      The whole keys must be equivalent to the key of the `source` relation,
     *      and each `value` cannot include any variables declared in `columns.destination` 
     * @param condition the extra join condition expression,
     *      can include variables declared in `columns.destination`
     * @param creator the object creator for internal elements
     */
    explicit join_find(
            operator_kind_type operator_kind,
            descriptor::relation source,
            std::vector<column, util::object_allocator<column>> columns,
            std::vector<key, util::object_allocator<key>> keys,
            util::unique_object_ptr<scalar::expression> condition,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the join kind
     * @param source the external source relation, which represents right build input of join operation
     * @param columns the columns to be joined, on the source relation.
     *      Each `source` must be a column on the `source` relation,
     *      and `destination` represents a new column of the resulting relation
     * @param keys the join key pieces.
     *      The whole keys must be equivalent to the key of the `source` relation,
     *      and each `value` cannot include any variables declared in `columns.destination`s 
     * @param condition the extra join condition expression,
     *      can include variables declared in `columns.destination`
     */
    explicit join_find(
            operator_kind_type operator_kind,
            descriptor::relation source,
            std::initializer_list<column> columns,
            std::initializer_list<util::rvalue_reference_wrapper<key>> keys,
            util::rvalue_ptr<scalar::expression> condition = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit join_find(join_find const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit join_find(join_find&& other, util::object_creator creator);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] join_find* clone(util::object_creator creator) const& override;
    [[nodiscard]] join_find* clone(util::object_creator creator) && override;

    /**
     * @brief returns the left prove input port.
     * @return the left prove input port
     */
    [[nodiscard]] input_port_type& left() noexcept;

    /// @copydoc left()
    [[nodiscard]] input_port_type const& left() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the join kind.
     * @return the join kind
     */
    [[nodiscard]] operator_kind_type operator_kind() const noexcept;

    /**
     * @brief sets the join kind.
     * @param operator_kind the join kind
     * @return this
     */
    join_find& operator_kind(operator_kind_type operator_kind) noexcept;

    /**
     * @brief returns the external source relation (as build input).
     * @return the source relation
     */
    [[nodiscard]] descriptor::relation& source() noexcept;

    /// @copydoc source()
    [[nodiscard]] descriptor::relation const& source() const noexcept;

    /**
     * @brief returns the target columns to scan.
     * @return the target columns
     */
    [[nodiscard]] std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns the storage keys and prove values.
     * @return pairs of storage keys and prove values
     */
    [[nodiscard]] tree::tree_fragment_vector<key>& keys() noexcept;

    /// @copydoc keys()
    [[nodiscard]] tree::tree_fragment_vector<key> const& keys() const noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    [[nodiscard]] util::optional_ptr<scalar::expression> condition() noexcept;

    /// @copydoc condition()
    [[nodiscard]] util::optional_ptr<scalar::expression const> condition() const noexcept;

    /**
     * @brief releases the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    [[nodiscard]] util::unique_object_ptr<scalar::expression> release_condition() noexcept;

    /**
     * @brief sets the condition expression.
     * @param condition the replacement
     * @return this
     */
    join_find& condition(util::unique_object_ptr<scalar::expression> condition) noexcept;

    /**
     * @brief returns ownership of the condition expression.
     * @return the condition expression
     */
    [[nodiscard]] util::object_ownership_reference<scalar::expression> ownership_condition() noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(join_find const& a, join_find const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(join_find const& a, join_find const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, join_find const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type left_;
    output_port_type output_;
    operator_kind_type operator_kind_;
    descriptor::relation source_;
    std::vector<column, util::object_allocator<column>> columns_;
    tree::tree_fragment_vector<key> keys_;
    util::unique_object_ptr<scalar::expression> condition_;
};

/**
 * @brief type_of for join_scan_point.
 */
template<> struct type_of<join_find::tag> : util::meta_type<join_find> {};

} // namespace takatori::relation
