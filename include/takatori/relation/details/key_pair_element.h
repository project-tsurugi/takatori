#pragma once

#include <functional>
#include <iostream>

#include <takatori/descriptor/variable.h>

namespace takatori::relation::details {

/**
 * @brief represents an element of distinguish key pair.
 */
class key_pair_element {
public:
    /**
     * @brief creates a new instance.
     * @param left the key column of the first (left) input relation
     * @param right the key column of the second (right) input relation
     */
    key_pair_element(descriptor::variable left, descriptor::variable right) noexcept;

    /**
     * @brief returns the key column of the first input relation.
     * @return the descriptor of column in the first input
     */
    [[nodiscard]] descriptor::variable& left() noexcept;

    /// @copydoc left()
    [[nodiscard]] descriptor::variable const& left() const noexcept;

    /**
     * @brief returns the key column of the second input relation.
     * @return the descriptor of column in the second input
     */
    [[nodiscard]] descriptor::variable& right() noexcept;

    /// @copydoc right()
    [[nodiscard]] descriptor::variable const& right() const noexcept;

private:
    descriptor::variable left_;
    descriptor::variable right_;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(key_pair_element const& a, key_pair_element const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(key_pair_element const& a, key_pair_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, key_pair_element const& value);

} // namespace takatori::relation::details

/**
 * @brief std::hash specialization for takatori::relation::details::union_mapping.
 */
template<>
struct std::hash<takatori::relation::details::key_pair_element> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::relation::details::key_pair_element const& value) const noexcept;
};
