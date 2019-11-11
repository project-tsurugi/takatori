#pragma once

#include <iostream>
#include <memory>
#include <utility>

#include "descriptor_kind.h"

namespace takatori::descriptor {

/**
 * @brief a program element descriptors.
 * @tparam Kind the element kind
 */
template<descriptor_kind Kind>
class element_descriptor {
public:
    /// @brief the descriptor kind
    static constexpr inline descriptor_kind tag = Kind;

    /**
     * @brief creates a new instance.
     * @param entity the entity
     */
    explicit element_descriptor(std::shared_ptr<void> entity) noexcept : entity_(std::move(entity)) {}

    /**
     * @brief returns the descriptor entity.
     * @return the descriptor entity
     */
    std::shared_ptr<void> const& entity() const noexcept { return entity_; }

private:
    std::shared_ptr<void> entity_;
};

/**
 * @brief describes a value.
 */
using value_descriptor = element_descriptor<descriptor_kind::value>;

/**
 * @brief describes a variable.
 */
using variable_descriptor = element_descriptor<descriptor_kind::variable>;

/**
 * @brief describes a function.
 */
using function_descriptor = element_descriptor<descriptor_kind::function>;

/**
 * @brief describes a type.
 */
using type_descriptor = element_descriptor<descriptor_kind::type>;

/**
 * @brief returns whether or not the each element which the descriptor indicates is same.
 * @tparam Kind the element kind
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if the two descriptors are equivalent
 * @return false otherwise
 */
template<descriptor_kind Kind>
inline bool operator==(element_descriptor<Kind> const& a, element_descriptor<Kind> const& b) noexcept {
    return a.entity() == b.entity();
}

/**
 * @brief returns whether or not the each element which the descriptor indicates is not same.
 * @tparam Kind the element kind
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if the two descriptors are different
 * @return false otherwise
 */
template<descriptor_kind Kind>
inline bool operator!=(element_descriptor<Kind> const& a, element_descriptor<Kind> const& b) noexcept {
    return a.entity() != b.entity();
}

/**
 * @brief compares the pointer of two elements which individual descriptor indicate.
 * @tparam Kind the element kind
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if a < b
 * @return false otherwise
 */
template<descriptor_kind Kind>
inline bool operator<(element_descriptor<Kind> const& a, element_descriptor<Kind> const& b) noexcept {
    return a.entity() < b.entity();
}

/**
 * @brief compares the pointer of two elements which individual descriptor indicate.
 * @tparam Kind the element kind
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if a > b
 * @return false otherwise
 */
template<descriptor_kind Kind>
inline bool operator>(element_descriptor<Kind> const& a, element_descriptor<Kind> const& b) noexcept {
    return a.entity() > b.entity();
}

/**
 * @brief compares the pointer of two elements which individual descriptor indicate.
 * @tparam Kind the element kind
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if a <= b
 * @return false otherwise
 */
template<descriptor_kind Kind>
inline bool operator<=(element_descriptor<Kind> const& a, element_descriptor<Kind> const& b) noexcept {
    return a.entity() <= b.entity();
}

/**
 * @brief compares the pointer of two elements which individual descriptor indicate.
 * @tparam Kind the element kind
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if a >= b
 * @return false otherwise
 */
template<descriptor_kind Kind>
inline bool operator>=(element_descriptor<Kind> const& a, element_descriptor<Kind> const& b) noexcept {
    return a.entity() >= b.entity();
}

/**
 * @brief appends string representation of the given value.
 * @tparam Kind the element kind
 * @param out the target output
 * @param value the target value
 * @return the output stream
 */
template<descriptor_kind Kind>
inline std::ostream& operator<<(std::ostream& out, element_descriptor<Kind> const& value) {
    return out << Kind << "(" << value.entity().get() << ")";
}

} // namespace takatori::descriptor
