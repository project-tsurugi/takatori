#pragma once

#include <initializer_list>
#include <vector>

#include "type_kind.h"
#include "data.h"
#include "details/table_column.h"

#include <takatori/util/meta_type.h>

namespace takatori::type {

/**
 * @brief a compiler synthesized type for generated row identifiers.
 */
class table : public data {
public:
    /// @brief the kind of this type.
    static constexpr type_kind tag = type_kind::table;

    /// @brief the column type.
    using column_type = details::table_column;

    table() = delete;

    /**
     * @brief creates a new instance.
     * @param columns the columns of the table
     */
    explicit table(std::vector<column_type> columns) noexcept;

    /**
     * @brief creates a new instance.
     * @param columns the columns of the table
     */
    table(std::initializer_list<column_type> columns);

    ~table() override = default;
    table(table const& other) = delete;
    table& operator=(table const& other) = delete;
    table(table&& other) noexcept = delete;
    table& operator=(table&& other) noexcept = delete;

    [[nodiscard]] type_kind kind() const noexcept override;
    [[nodiscard]] table* clone() const& override;
    [[nodiscard]] table* clone() && override;

    /**
     * @brief returns the columns of the table.
     * @return the columns
     */
    [[nodiscard]] std::vector<column_type> const& columns() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(table const& a, table const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(table const& a, table const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, table const& value);

protected:
    [[nodiscard]] bool equals(data const& other) const noexcept override;
    [[nodiscard]] std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::vector<column_type> columns_;
};

template<> struct type_of<table::tag> : util::meta_type<table> {};

} // namespace takatori::type

/// @brief provides hash code of takatori::type::table.
template<> struct std::hash<takatori::type::table> : hash<takatori::type::data> {};
