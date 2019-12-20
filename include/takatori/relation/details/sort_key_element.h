#pragma once

#include <functional>
#include <iostream>

#include "../sort_direction.h"

#include <takatori/descriptor/variable.h>

namespace takatori::relation::details {

/**
 * @brief a sort key element on relation.
 */
class sort_key_element {
public:
    /**
     * @brief creates a new instance.
     * @param variable the sort key column on the relation
     * @param direction the sort direction of this key element
     */
    sort_key_element( // NOLINT
            descriptor::variable variable,
            sort_direction direction = sort_direction::ascendant) noexcept;

    /**
     * @brief returns the sort key column on the relation.
     * @return the variable descriptor of sort key
     */
    descriptor::variable const& variable() const noexcept;

    /**
     * @brief sets a sort key column on the relation.
     * @param variable the variable descriptor of sort key
     * @return this
     */
    sort_key_element& variable(descriptor::variable variable) noexcept;

    /**
     * @brief returns the sort direction of this key element.
     * @return the sort direction
     */
    sort_direction const& direction() const noexcept;

    /**
     * @brief sets a sort direction of this key element.
     * @param direction the sort direction
     * @return this
     */
    sort_key_element& direction(sort_direction direction) noexcept;

private:
    descriptor::variable variable_;
    sort_direction direction_;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(sort_key_element const& a, sort_key_element const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(sort_key_element const& a, sort_key_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, sort_key_element const& value);

} // namespace takatori::relation::details

/**
 * @brief std::hash specialization for takatori::relation::details::sort_key_element.
 */
template<>
struct std::hash<takatori::relation::details::sort_key_element> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::relation::details::sort_key_element const& value) const noexcept;
};
