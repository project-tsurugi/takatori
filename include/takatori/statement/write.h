#pragma once

#include <ostream>

#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/variable.h>
#include <takatori/scalar/expression.h>
#include <takatori/relation/write_kind.h>
#include <takatori/tree/tree_fragment_vector.h>
#include <takatori/util/rvalue_reference_wrapper.h>

#include "statement.h"
#include "statement_kind.h"
#include "details/write_tuple.h"

namespace takatori::statement {

using ::takatori::relation::write_kind;

/**
 * @brief write statement.
 */
class write final : public statement {
public:
    /// @brief the write operation kind
    using operator_kind_type = write_kind;

    /// @brief table column descriptor.
    using column = descriptor::variable;

    /// @brief tuple of each column value.
    using tuple = details::write_tuple;

    /// @brief the kind of this statement.
    static constexpr inline statement_kind tag = statement_kind::write;

    /**
     * @brief creates a new instance.
     * @param operator_kind the write operation kind
     * @param destination the target external relation, must represent an index to distinguish individual entries
     * @param columns the destination columns
     * @param tuples the each row data; must be ordered by `columns`
     */
    explicit write(
            operator_kind_type operator_kind,
            descriptor::relation destination,
            std::vector<column, util::object_allocator<column>> columns,
            std::vector<tuple, util::object_allocator<tuple>> tuples) noexcept;

    /**
     * @brief creates a new instance.
     * @param operator_kind the write operation kind
     * @param destination the target external relation, must represent an index to distinguish individual entries
     * @param columns the destination columns
     * @param tuples the each row data; must be ordered by `columns`
     */
    explicit write(
            operator_kind_type operator_kind,
            descriptor::relation destination,
            std::initializer_list<column> columns,
            std::initializer_list<std::initializer_list<util::rvalue_reference_wrapper<scalar::expression>>> tuples);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit write(write const& other, util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit write(write&& other, util::object_creator creator) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] write* clone(util::object_creator creator) const& override;
    [[nodiscard]] write* clone(util::object_creator creator) && override;

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
     * @brief returns the columns on the destination table.
     * @return This designates each mapping between table column and its value to write.
     *      Each elements in the individual tuples() must be ordered by this sequence of columns.
     */
    [[nodiscard]] std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns the tuples to write.
     * @details The individual elements in each tuple must be ordered by columns().
     * @return the value tuples
     */
    [[nodiscard]] tree::tree_fragment_vector<tuple>& tuples() noexcept;

    /// @copydoc tuples()
    [[nodiscard]] tree::tree_fragment_vector<tuple> const& tuples() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(write const& a, write const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
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
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    operator_kind_type operator_kind_;
    descriptor::relation destination_;
    std::vector<column, util::object_allocator<column>> columns_;
    tree::tree_fragment_vector<tuple> tuples_;
};

} // namespace takatori::statement
