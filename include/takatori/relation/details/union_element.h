#pragma once

#include <functional>
#include <iostream>
#include <optional>

#include <takatori/descriptor/variable.h>

namespace takatori::relation::details {

/**
 * @brief represents a column mapping in union operation.
 */
class union_element {
public:
    /// @brief the source column type.
    using source_type = descriptor::variable;
    
    /// @brief the destination column type.
    using destination_type = descriptor::variable;
    
    /**
     * @brief creates a new instance.
     * @param left the source column of the first (left) input relation
     * @param right the source column of the second (right) input relation
     * @param destination the destination column of the output relation
     */
    union_element(
            std::optional<source_type> left,
            std::optional<source_type> right,
            destination_type destination) noexcept;

    /**
     * @brief returns the source column of the first input relation.
     * @return the descriptor of column in the first input
     * @return empty if this column omits the first input
     */
    std::optional<source_type>& left() noexcept;

    /// @copydoc left()
    std::optional<source_type> const& left() const noexcept;

    /**
     * @brief returns the source column of the second input relation.
     * @return the descriptor of column in the second input
     * @return empty if this column omits the second input
     */
    std::optional<source_type>& right() noexcept;

    /// @copydoc right()
    std::optional<source_type> const& right() const noexcept;

    /**
     * @brief returns the descriptor of destination column.
     * @return the destination column
     */
    destination_type& destination() noexcept;

    /// @copydoc destination()
    destination_type const& destination() const noexcept;

private:
    std::optional<source_type> left_;
    std::optional<source_type> right_;
    destination_type destination_;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(union_element const& a, union_element const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(union_element const& a, union_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, union_element const& value);

} // namespace takatori::relation::details

/**
 * @brief std::hash specialization for takatori::relation::details::union_mapping.
 */
template<>
struct std::hash<takatori::relation::details::union_element> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::relation::details::union_element const& value) const noexcept;
};
