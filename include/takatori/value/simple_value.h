#pragma once

#include "data.h"
#include "value_kind.h"

#include "takatori/util/comparable_traits.h"
#include "takatori/util/downcast.h"
#include "takatori/util/hash.h"
#include "takatori/util/meta_type.h"

namespace takatori::value {

/**
 * @brief traits for the simple_value.
 * This provides the following properties:
 * - entity_type the entity type
 * - view_type the view type
 * @tparam Kind the value kind, must be simple type
 */
template<value_kind Kind> struct simple_value_traits;

/**
 * @brief a model template of simple values, which have no value specific information.
 * @tparam Kind the value kind
 * @tparam Entity the entity type
 */
template<value_kind Kind>
class simple_value : public data {
public:
    /// @brief the kind of this value.
    static constexpr inline value_kind tag = Kind;

    /// @brief the entity type
    using entity_type = typename simple_value_traits<Kind>::entity_type;

    /// @brief the view type
    using view_type = typename simple_value_traits<Kind>::view_type;

    static_assert(std::is_copy_constructible_v<entity_type>);

    static_assert(util::is_equal_comparable_v<entity_type, entity_type>);
    static_assert(util::is_less_comparable_v<entity_type, entity_type>);
    static_assert(util::is_hash_support_v<entity_type>);

    /**
     * @brief creates a new object.
     */
    explicit constexpr simple_value(entity_type entity = {}) noexcept : entity_(std::move(entity)) {};

    ~simple_value() override = default;
    simple_value(simple_value const& other) = delete;
    simple_value& operator=(simple_value const& other) = delete;
    simple_value(simple_value&& other) noexcept = delete;
    simple_value& operator=(simple_value&& other) noexcept = delete;

    value_kind kind() const noexcept override;
    simple_value* clone(util::object_creator creator) const& override;
    simple_value* clone(util::object_creator creator) && override;

    /**
     * @brief returns the entity value.
     * @return the entity value
     */
    constexpr view_type get() const noexcept;

    /// @copydoc get()
    explicit constexpr operator view_type() const noexcept;

protected:
    bool equals(data const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    entity_type entity_;
};

template<value_kind Kind>
inline value_kind
simple_value<Kind>::kind() const noexcept {
    return Kind;
}

template<value_kind Kind>
inline simple_value<Kind>*
simple_value<Kind>::clone(util::object_creator creator) const& {
    return creator.create_object<simple_value<Kind>>(entity_);
}

template<value_kind Kind>
inline simple_value<Kind>*
simple_value<Kind>::clone(util::object_creator creator) && {
    return creator.create_object<simple_value<Kind>>(std::move(entity_));
}

template<value_kind Kind>
inline constexpr typename simple_value<Kind>::view_type
simple_value<Kind>::get() const noexcept {
    return entity_;
}

template<value_kind Kind>
inline constexpr
simple_value<Kind>::operator view_type() const noexcept {
    return get();
}

/**
 * @brief returns whether or not the two elements are equivalent.
 * @tparam Kind the value kind
 * @param a the first element
 * @param b the second element
 * @return true if the both entities are equivalent
 * @return false otherwise
 */
template<value_kind Kind>
inline constexpr bool
operator==(simple_value<Kind> const& a, simple_value<Kind> const& b) noexcept {
    return a.get() == b.get();
}

/**
 * @brief returns whether or not the two elements are different.
 * @tparam Kind the value kind
 * @param a the first element
 * @param b the second element
 * @return true if the both entities are different
 * @return false otherwise
 */
template<value_kind Kind>
inline constexpr bool
operator!=(simple_value<Kind> const& a, simple_value<Kind> const& b) noexcept {
    return !(a == b);
}

/**
 * @brief appends string representation of the given value.
 * @tparam Kind the value kind
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<value_kind Kind>
inline std::ostream&
operator<<(std::ostream& out, simple_value<Kind> const& value) {
    return out << Kind << "(" << value.get() << ")";
}

template<value_kind Kind>
inline bool
simple_value<Kind>::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<simple_value<Kind>>(other);
}

template<value_kind Kind>
inline std::size_t
simple_value<Kind>::hash() const noexcept {
    return std::hash<entity_type>{}(entity_);
}

template<value_kind Kind>
inline std::ostream&
simple_value<Kind>::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::value

/**
 * @brief provides hash code of takatori::value::simple_value.
 * @tparam Kind the value kind
 */
template<takatori::value::value_kind Kind>
struct std::hash<takatori::value::simple_value<Kind>> : hash<takatori::value::data> {};
