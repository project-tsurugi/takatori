#pragma once

#include <ostream>
#include <utility>

#include "descriptor_kind.h"
#include "binding_info.h"

#include <takatori/util/hash.h>
#include <takatori/util/optional_ptr.h>

namespace takatori::descriptor {

/**
 * @brief a light weight reference of descriptors.
 * @details This has the same equivalence to the target descriptor,
 *  that is, each descriptor `u` and `v` satisfies
 *  `u == v => u == v.reference()`.
 * @tparam T the descriptor type
 */
template<class T>
class reference {
public:
    /// @brief the descriptor type.
    using descriptor_type = T;

    /// @brief the entity type.
    using entity_type = typename descriptor_type::entity_type;

    /// @brief the descriptor kind
    static constexpr inline descriptor_kind tag = descriptor_type::tag;

    /**
     * @brief creates a new instance which refers an empty descriptor.
     */
    constexpr reference() noexcept = default;

    /**
     * @brief creates a new instance.
     * @param target the target descriptor
     */
    explicit reference(T const& target) noexcept :
        entity_ { target.optional_entity().get() }
    {}

    /**
     * @brief returns the descriptor entity.
     * @return the descriptor entity
     * @warning undefined behavior if the entity is absent, or already disposed
     */
    [[nodiscard]] constexpr entity_type& entity() const noexcept {
        return *optional_entity();
    }

    /**
     * @brief returns the descriptor entity.
     * @return the descriptor entity
     * @return empty if the entity is absent
     * @warning undefined behavior if the entity is or already disposed
     */
    [[nodiscard]] constexpr util::optional_ptr<entity_type> optional_entity() const noexcept {
        return util::optional_ptr { entity_ };
    }

private:
    entity_type* entity_ {};
};

/**
 * @brief returns whether or not the each element which the descriptor indicates is same.
 * @tparam T the descriptor type
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if the two descriptors are equivalent
 * @return false otherwise
 */
template<class T>
inline bool operator==(reference<T> const& a, reference<T> const& b) noexcept {
    return a.optional_entity() == b.optional_entity();
}

/**
 * @brief returns whether or not the each element which the descriptor indicates is not same.
 * @tparam T the descriptor type
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if the two descriptors are different
 * @return false otherwise
 */
template<class T>
inline bool operator!=(reference<T> const& a, reference<T> const& b) noexcept {
    return !(a == b);
}

/**
 * @brief appends string representation of the given value.
 * @tparam T the descriptor type
 * @param out the target output
 * @param value the target value
 * @return the output stream
 */
template<class T>
inline std::ostream& operator<<(std::ostream& out, reference<T> const& value) {
    return out << T::tag << "(" << value.optional_entity() << ")";
}

} // namespace takatori::descriptor

/**
 * @brief std::hash specialization for takatori::descriptor::reference.
 * @tparam T the descriptor type
 */
template<class T>
struct std::hash<takatori::descriptor::reference<T>> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::descriptor::reference<T> const& value) const noexcept {
        return ::takatori::util::hash(T::tag, value.optional_entity());
    }
};
