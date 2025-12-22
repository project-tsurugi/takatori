#pragma once

#include <ostream>

#include <takatori/descriptor/variable.h>

namespace takatori::relation::details {

/**
 * @brief represents a column mapping between relations.
 */
class apply_column {
public:
    /// @brief the column position type.
    using position_type = std::size_t;

    /// @brief the column variable type.
    using variable_type = descriptor::variable;
    
    /**
     * @brief creates a new instance.
     * @param position the position of source column (0-origin)
     * @param variable the destination column variable
     */
    apply_column(position_type position, variable_type variable) noexcept;

    /**
     * @brief returns the column position in the output table of the table-valued function (0-origin).
     * @return the column position (0-origin).
     */
    [[nodiscard]] position_type position() const noexcept;

    /**
      * @brief sets the column position in the output table of the table-valued function (0-origin).
      * @param position the column position (0-origin).
      * @return this
      */
    [[nodiscard]] apply_column& position(position_type position) noexcept;

    /**
     * @brief returns the variable descriptor of the output column.
     * @return the variable descriptor.
     */
    [[nodiscard]] variable_type& variable() noexcept;

    /// @copydoc variable()
    [[nodiscard]] variable_type const& variable() const noexcept;

private:
    position_type position_;
    variable_type variable_;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(apply_column const& a, apply_column const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(apply_column const& a, apply_column const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, apply_column const& value);

} // namespace takatori::relation::details

/**
 * @brief std::hash specialization for takatori::relation::details::apply_column.
 */
template<>
struct std::hash<takatori::relation::details::apply_column> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::relation::details::apply_column const& value) const noexcept;
};
