#pragma once

#include "data.h"
#include "type_kind.h"

#include "takatori/util/downcast.h"

namespace takatori::type {

/**
 * @brief a model template of simple types, which have no type specific information.
 * @tparam Kind the type kind
 */
template<type_kind Kind>
class simple_type : public data {

    // FIXME: restrict kind

public:
    /// @brief the kind of this type.
    static constexpr inline type_kind tag = Kind;

    /**
     * @brief creates a new object.
     */
    explicit constexpr simple_type() = default;

    ~simple_type() override = default;
    simple_type(simple_type const& other) = delete;
    simple_type& operator=(simple_type const& other) = delete;
    simple_type(simple_type&& other) noexcept = delete;
    simple_type& operator=(simple_type&& other) noexcept = delete;

    type_kind kind() const noexcept override;
    simple_type* clone(util::object_creator creator) const& override;
    simple_type* clone(util::object_creator creator) && override;

protected:
    bool equals(data const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;
};

template<type_kind Kind>
inline type_kind
simple_type<Kind>::kind() const noexcept {
    return Kind;
}

template<type_kind Kind>
inline simple_type<Kind>*
simple_type<Kind>::clone(util::object_creator creator) const& {
    return creator.create_object<simple_type<Kind>>();
}

template<type_kind Kind>
inline simple_type<Kind>*
simple_type<Kind>::clone(util::object_creator creator) && {
    return creator.create_object<simple_type<Kind>>();
}

/**
 * @brief returns whether or not the two elements are equivalent.
 * @tparam Kind the type kind
 * @param a the first element
 * @param b the second element
 * @return always true
 */
template<type_kind Kind>
inline constexpr bool operator==(simple_type<Kind> const& a, simple_type<Kind> const& b) noexcept {
    (void) a;
    (void) b;
    return true;
}

/**
 * @brief returns whether or not the two elements are different.
 * @tparam Kind the type kind
 * @param a the first element
 * @param b the second element
 * @return always false
 */
template<type_kind Kind>
inline constexpr bool operator!=(simple_type<Kind> const& a, simple_type<Kind> const& b) noexcept {
    (void) a;
    (void) b;
    return false;
}

/**
 * @brief appends string representation of the given value.
 * @tparam Kind the type kind
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<type_kind Kind>
inline std::ostream& operator<<(std::ostream& out, simple_type<Kind> const& value) {
    (void) value;
    return out << Kind << "()";
}

template<type_kind Kind>
inline bool simple_type<Kind>::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<simple_type<Kind>>(other);
}

template<type_kind Kind>
inline std::size_t simple_type<Kind>::hash() const noexcept {
    return 0;
}

template<type_kind Kind>
inline std::ostream& simple_type<Kind>::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type

namespace std {

/**
 * @brief provides hash code of takatori::type::simple_type.
 * @tparam Kind the type kind
 */
template<takatori::type::type_kind Kind>
struct hash<takatori::type::simple_type<Kind>> : hash<takatori::type::data> {};

} // namespace std
