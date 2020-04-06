#pragma once

#include <functional>
#include <iostream>

#include <takatori/descriptor/variable.h>

namespace takatori::relation::details {

/**
 * @brief represents a column mapping between relations.
 */
class mapping_element {
public:
    /// @brief the source column type.
    using source_type = descriptor::variable;
    
    /// @brief the destination column type.
    using destination_type = descriptor::variable;
    
    /**
     * @brief creates a new instance.
     * @param source the source column descriptor
     * @param destination the destination column descriptor
     */
    mapping_element(source_type source, destination_type destination) noexcept;

    /**
     * @brief returns the descriptor of source column.
     * @return the source column
     */
    source_type& source() noexcept;

    /// @copydoc source()
    source_type const& source() const noexcept;

    /**
     * @brief returns the descriptor of destination column.
     * @return the destination column
     */
    destination_type& destination() noexcept;

    /// @copydoc destination()
    destination_type const& destination() const noexcept;

private:
    source_type source_;
    destination_type destination_;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(mapping_element const& a, mapping_element const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(mapping_element const& a, mapping_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, mapping_element const& value);

} // namespace takatori::relation::details

/**
 * @brief std::hash specialization for takatori::relation::details::mapping_element.
 */
template<>
struct std::hash<takatori::relation::details::mapping_element> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::relation::details::mapping_element const& value) const noexcept;
};
