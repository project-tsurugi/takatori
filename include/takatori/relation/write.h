#pragma once

#include <initializer_list>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "write_kind.h"
#include "details/mapping_element.h"

#include <takatori/descriptor/relation.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation {

/**
 * @brief writes rows into the table.
 * @details This operation also updates the corresponded index entries.
 */
class write : public expression {
public:
    /// @brief the write operation kind
    using operator_kind_type = write_kind;

    /// @brief column mapping between the input relation and the destination table.
    using key = details::mapping_element;

    /// @brief column mapping between the input relation and the destination table.
    using column = details::mapping_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::write;

    /**
     * @brief creates a new object.
     * @param operator_kind the write operation kind
     * @param destination the target external relation, must represent an index to distinguish individual entries
     * @param keys the column mappings between the input relation and the destination table for identifying target rows
     * @param columns the column mappings between the input relation and the destination table for setting row contents
     * @param creator the object creator for internal elements
     */
    explicit write(
            operator_kind_type operator_kind,
            descriptor::relation destination,
            std::vector<key, util::object_allocator<key>> keys,
            std::vector<column, util::object_allocator<column>> columns,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the write operation kind
     * @param destination the target external relation, must represent an index to distinguish individual entries
     * @param keys the column mappings between the input relation and the destination table for identifying target rows
     * @param columns the column mappings between the input relation and the destination table for setting row contents
     */
    explicit write(
            operator_kind_type operator_kind,
            descriptor::relation destination,
            std::initializer_list<key> keys,
            std::initializer_list<column> columns);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit write(write const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit write(write&& other, util::object_creator creator);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] write* clone(util::object_creator creator) const& override;
    [[nodiscard]] write* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * @return the input port
     */
    [[nodiscard]] input_port_type& input() noexcept;

    /// @copydoc input()
    [[nodiscard]] input_port_type const& input() const noexcept;

    /**
     * @brief returns the operator kind.
     * @return operator kind
     */
    [[nodiscard]] operator_kind_type operator_kind() const noexcept;

    /**
     * @brief sets operator kind.
     * @param operator_kind operator kind
     * @return this
     */
    [[nodiscard]] write& operator_kind(operator_kind_type operator_kind) noexcept;

    /**
     * @brief returns the destination relation.
     * @return the destination relation
     */
    [[nodiscard]] descriptor::relation& destination() noexcept;

    /// @brief destination()
    [[nodiscard]] descriptor::relation const& destination() const noexcept;

    /**
     * @brief returns write target keys in the input relation.
     * @return the write target keys
     */
    [[nodiscard]] std::vector<key, util::object_allocator<key>>& keys() noexcept;

    /// @copydoc keys()
    [[nodiscard]] std::vector<key, util::object_allocator<key>> const& keys() const noexcept;

    /**
     * @brief returns column values to write in the input relation.
     * @return the column values
     */
    [[nodiscard]] std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(write const& a, write const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(write const& a, write const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, write const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    operator_kind_type operator_kind_;
    descriptor::relation destination_;
    std::vector<key, util::object_allocator<key>> keys_;
    std::vector<column, util::object_allocator<column>> columns_;
};

/**
 * @brief type_of for write.
 */
template<> struct type_of<write::tag> : util::meta_type<write> {};

} // namespace takatori::relation
