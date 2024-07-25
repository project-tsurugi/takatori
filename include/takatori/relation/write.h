#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "expression.h"
#include "expression_kind.h"
#include "write_kind.h"
#include "details/mapping_element.h"

#include <takatori/descriptor/relation.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

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

    /// @brief column on the destination table.
    using default_column = descriptor::variable;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::write;

    /**
     * @brief creates a new object.
     * @param operator_kind the write operation kind
     * @param destination the target external relation, must represent an index to distinguish individual entries
     * @param keys the column mappings between the input relation and the destination table for identifying target rows
     * @param columns the column mappings between the input relation and the destination table for setting row contents
     * @param default_columns list of the columns to set each default value
     */
    explicit write(
            operator_kind_type operator_kind,
            descriptor::relation destination,
            std::vector<key> keys,
            std::vector<column> columns,
            std::vector<default_column> default_columns = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param operator_kind the write operation kind
     * @param destination the target external relation, must represent an index to distinguish individual entries
     * @param keys the column mappings between the input relation and the destination table for identifying target rows
     * @param columns the column mappings between the input relation and the destination table for setting row contents
     * @param default_columns list of the columns to set each default value
     */
    explicit write(
            operator_kind_type operator_kind,
            descriptor::relation destination,
            std::initializer_list<key> keys,
            std::initializer_list<column> columns,
            std::initializer_list<default_column> default_columns = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit write(util::clone_tag_t, write const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit write(util::clone_tag_t, write&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] write* clone() const& override;
    [[nodiscard]] write* clone() && override;

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
    [[nodiscard]] std::vector<key>& keys() noexcept;

    /// @copydoc keys()
    [[nodiscard]] std::vector<key> const& keys() const noexcept;

    /**
     * @brief returns column values to write in the input relation.
     * @return the column values
     */
    [[nodiscard]] std::vector<column>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column> const& columns() const noexcept;

    /**
     * @brief returns the columns that should be written each default value.
     * @return the columns that should be written each default value.
     * @note this only includes the columns explicitly specified to be set to default values (e.g., specified `DEFAULT`).
     * @note this may include columns that marked as "read-only."
     */
    [[nodiscard]] std::vector<default_column>& default_columns() noexcept;

    /// @copydoc default_columns()
    [[nodiscard]] std::vector<default_column> const& default_columns() const noexcept;

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
    std::vector<key> keys_;
    std::vector<column> columns_;
    std::vector<default_column> default_columns_;
};

/**
 * @brief type_of for write.
 */
template<> struct type_of<write::tag> : util::meta_type<write> {};

} // namespace takatori::relation
