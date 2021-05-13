#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "expression.h"
#include "expression_kind.h"

#include "details/search_key_element.h"
#include "details/mapping_element.h"

#include <takatori/tree/tree_fragment_vector.h>

#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/variable.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>
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
     * @param columns the target columns to scan.
     *      Each `source` must be a column on the `source` relation,
     *      and `destination` represents a new column of the resulting relation
     * @param keys the pieces of key to point to target row.
     *      The whole keys must be equivalent to the key of the `source` relation,
     *      and each `value` cannot include any variables declared in `columns.destination`s 
     */
    explicit find(
            descriptor::relation source,
            std::vector<column> columns,
            std::vector<key> keys) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the source relation
     * @param columns the target columns to scan.
     *      Each `source` must be a column on the `source` relation,
     *      and `destination` represents a new column of the resulting relation
     * @param keys the pieces of key to point to target row.
     *      The whole keys must be equivalent to the key of the `source` relation,
     *      and each `value` cannot include any variables declared in `columns.destination`s 
     * @attention this may take copies of arguments
     */
    explicit find(
            descriptor::relation source,
            std::initializer_list<column> columns,
            std::initializer_list<util::rvalue_reference_wrapper<key>> keys);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit find(util::clone_tag_t, find const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit find(util::clone_tag_t, find&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] find* clone() const& override;
    [[nodiscard]] find* clone() && override;

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
     * @brief returns pieces of key to point to the target row.
     * @return the target row key
     */
    [[nodiscard]] tree::tree_fragment_vector<key>& keys() noexcept;

    /// @brief keys()
    [[nodiscard]] tree::tree_fragment_vector<key> const& keys() const noexcept;

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
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    output_port_type output_;
    descriptor::relation source_;
    std::vector<column> columns_;
    tree::tree_fragment_vector<key> keys_;
};

/**
 * @brief type_of for find.
 */
template<> struct type_of<find::tag> : util::meta_type<find> {};

} // namespace takatori::relation
