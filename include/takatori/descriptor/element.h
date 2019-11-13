#pragma once

#include <iostream>
#include <memory>
#include <utility>

#include "descriptor_kind.h"

namespace takatori::descriptor {

/**
 * @brief a program element descriptors.
 * @tparam Kind the element kind
 * @tparam Entity the entity type
 */
template<descriptor_kind Kind, class Entity>
class element {
public:
    /// @brief the entity type.
    using entity_type = Entity;

    /// @brief the descriptor kind
    static constexpr inline descriptor_kind tag = Kind;

    /**
     * @brief creates a new instance.
     * @param entity the entity
     */
    explicit element(std::shared_ptr<entity_type> entity) noexcept : entity_(std::move(entity)) {}

    /**
     * @brief returns the descriptor entity.
     * @return the descriptor entity
     */
    std::shared_ptr<entity_type> const& entity() const noexcept { return entity_; }

private:
    std::shared_ptr<entity_type> entity_;
};

/**
 * @brief returns whether or not the each element which the descriptor indicates is same.
 * @tparam Kind the element kind
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if the two descriptors are equivalent
 * @return false otherwise
 */
template<descriptor_kind K, class E>
inline bool operator==(element<K, E> const& a, element<K, E> const& b) noexcept {
    return a.entity() == b.entity();
}

/**
 * @brief returns whether or not the each element which the descriptor indicates is not same.
 * @tparam K the element kind
 * @tparam E the entity type
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if the two descriptors are different
 * @return false otherwise
 */
template<descriptor_kind K, class E>
inline bool operator!=(element<K, E> const& a, element<K, E> const& b) noexcept {
    return a.entity() != b.entity();
}

/**
 * @brief compares the pointer of two elements which individual descriptor indicate.
 * @tparam K the element kind
 * @tparam E the entity type
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if a < b
 * @return false otherwise
 */
template<descriptor_kind K, class E>
inline bool operator<(element<K, E> const& a, element<K, E> const& b) noexcept {
    return a.entity() < b.entity();
}

/**
 * @brief compares the pointer of two elements which individual descriptor indicate.
 * @tparam K the element kind
 * @tparam E the entity type
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if a > b
 * @return false otherwise
 */
template<descriptor_kind K, class E>
inline bool operator>(element<K, E> const& a, element<K, E> const& b) noexcept {
    return a.entity() > b.entity();
}

/**
 * @brief compares the pointer of two elements which individual descriptor indicate.
 * @tparam K the element kind
 * @tparam E the entity type
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if a <= b
 * @return false otherwise
 */
template<descriptor_kind K, class E>
inline bool operator<=(element<K, E> const& a, element<K, E> const& b) noexcept {
    return a.entity() <= b.entity();
}

/**
 * @brief compares the pointer of two elements which individual descriptor indicate.
 * @tparam K the element kind
 * @tparam E the entity type
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if a >= b
 * @return false otherwise
 */
template<descriptor_kind K, class E>
inline bool operator>=(element<K, E> const& a, element<K, E> const& b) noexcept {
    return a.entity() >= b.entity();
}

/**
 * @brief appends string representation of the given value.
 * @tparam K the element kind
 * @tparam E the entity type
 * @param out the target output
 * @param value the target value
 * @return the output stream
 */
template<descriptor_kind K, class E>
inline std::ostream& operator<<(std::ostream& out, element<K, E> const& value) {
    return out << K << "(" << value.entity().get() << ")";
}

} // namespace takatori::descriptor


/**
 * @brief std::hash specialization for takatori::descriptor::element.
 * @tparam K the element kind
 * @tparam E the entity type
 */
template<takatori::descriptor::descriptor_kind K, class E>
struct std::hash<takatori::descriptor::element<K, E>> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::descriptor::element<K, E> const& value) const noexcept {
        return reinterpret_cast<std::size_t>(value.entity().get()); // NOLINT
    }
};
