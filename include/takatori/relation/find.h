#pragma once

#include <initializer_list>
#include <vector>

#include "expression.h"
#include "expression_kind.h"

#include "details/search_key_element.h"
#include "details/mapping_element.h"

#include <takatori/tree/tree_fragment_vector.h>

#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/variable.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/rvalue_reference_wrapper.h>

namespace takatori::relation {

/**
 * @brief obtains rows from an external relation by their key.
 */
class find : public expression {
public:
    /// @brief the scan column type.
    using column = details::mapping_element;

    /// @brief the key piece type.
    using key = details::search_key_element<find>;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::find;

    /**
     * @brief creates a new instance.
     * @param source the source relation
     * @param columns the target columns to scan
     * @param keys the pieces of key to point to target row
     * @param creator the object creator for internal elements
     */
    explicit find(
            descriptor::relation source,
            std::vector<column, util::object_allocator<column>> columns,
            std::vector<key, util::object_allocator<key>> keys,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the source relation
     * @param columns the target columns to scan
     * @param keys the pieces of key to point to target row
     * @attention this may take copies of arguments
     */
    explicit find(
            descriptor::relation source,
            std::initializer_list<column> columns,
            std::initializer_list<util::rvalue_reference_wrapper<key>> keys);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit find(find const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit find(find&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    find* clone(util::object_creator creator) const& override;
    find* clone(util::object_creator creator) && override;

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
    find& source(descriptor::relation source) noexcept;

    /**
     * @brief returns the target columns to scan.
     * @return the target columns
     */
    std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns pieces of key to point to the target row.
     * @return the target row key
     */
    tree::tree_fragment_vector<key>& keys() noexcept;

    /// @brief keys()
    tree::tree_fragment_vector<key> const& keys() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(find const& a, find const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(find const& a, find const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, find const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    descriptor::relation source_;
    std::vector<column, util::object_allocator<column>> columns_;
    tree::tree_fragment_vector<key> keys_;
};

/**
 * @brief type_of for find.
 */
template<> struct type_of<find::tag> : util::meta_type<find> {};

} // namespace takatori::relation
