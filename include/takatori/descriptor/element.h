#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include "descriptor_kind.h"

#include <takatori/document/region.h>

#include <takatori/util/hash.h>
#include <takatori/util/object.h>
#include <takatori/util/optional_ptr.h>

namespace takatori::descriptor {

/**
 * @brief a program element descriptors.
 * @tparam Kind the element kind
 */
template<descriptor_kind Kind>
class element {
public:
    /// @brief the descriptor kind
    static constexpr inline descriptor_kind tag = Kind;

    /**
     * @brief creates a new instance.
     * @param entity the entity
     */
    explicit element(std::shared_ptr<util::object> entity) noexcept
        : entity_(std::move(entity))
    {}

    /**
     * @brief returns the descriptor entity.
     * @return the descriptor entity
     * @warning undefined behavior if the entity is absent
     */
    util::object& entity() const noexcept {
        return *entity_;
    }

    /**
     * @brief returns the descriptor entity.
     * @return the descriptor entity
     * @return empty if the entity is absent
     */
    util::optional_ptr<util::object> optional_entity() const noexcept {
        return util::optional_ptr { entity_.get() };
    }

    /**
     * @brief returns the descriptor entity.
     * @return the descriptor entity
     */
    std::shared_ptr<util::object> const& shared_entity() const noexcept {
        return entity_;
    }

    /**
     * @brief returns the document region of this element.
     * @return the document region
     */
    document::region& region() noexcept {
        return region_;
    }

    /// @copydoc region()
    document::region const& region() const noexcept {
        return region_;
    }

private:
    std::shared_ptr<util::object> entity_;
    document::region region_ {};
};

/**
 * @brief returns whether or not the each element which the descriptor indicates is same.
 * @tparam Kind the element kind
 * @param a the first descriptor
 * @param b the second descriptor
 * @return true if the two descriptors are equivalent
 * @return false otherwise
 */
template<descriptor_kind Kind>
inline bool operator==(element<Kind> const& a, element<Kind> const& b) noexcept {
    return a.optional_entity() == b.optional_entity();
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
inline bool operator!=(element<Kind> const& a, element<Kind> const& b) noexcept {
    return !(a == b);
}

/**
 * @brief appends string representation of the given value.
 * @tparam Kind the element kind
 * @param out the target output
 * @param value the target value
 * @return the output stream
 */
template<descriptor_kind Kind>
inline std::ostream& operator<<(std::ostream& out, element<Kind> const& value) {
    return out << Kind << "(" << value.optional_entity() << ")";
}

} // namespace takatori::descriptor

/**
 * @brief std::hash specialization for takatori::descriptor::element.
 * @tparam Kind the element kind
 */
template<takatori::descriptor::descriptor_kind Kind>
struct std::hash<takatori::descriptor::element<Kind>> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::descriptor::element<Kind> const& value) const noexcept {
        return ::takatori::util::hash(Kind, value.optional_entity());
    }
};
