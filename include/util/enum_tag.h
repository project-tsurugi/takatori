#pragma once

#include <iostream>

namespace util {

/**
 * @brief a tag that represents "default".
 * @tparam E the enum type
 */
template<class E>
struct enum_default_tag_t {};

/**
 * @brief a tag class for scoped enum values.
 * @tparam E the enum type
 * @tparam Kind the enum value
 */
template<class E, E Kind>
struct enum_tag_t : enum_default_tag_t<E> {

    /// @brief the enum type.
    using type = E;

    /// @brief the enum value.
    static inline constexpr type value = Kind;

    /**
     * @brief creates a new object.
     */
    explicit enum_tag_t() = default;

    /**
     * @brief returns the enum value.
     * @return the enum value
     */
    constexpr operator E() const noexcept { return value; } // NOLINT
};

/**
 * @brief a tag object for scoped enum values.
 * @tparam E the enum type
 * @tparam Kind the enum value
 */
template<class E, E Kind>
inline constexpr enum_tag_t<E, Kind> enum_tag {};

/**
 * @brief appends string representation of the given tag's value.
 * @tparam E the enum type
 * @tparam Kind the enum value
 * @param out the target output
 * @param tag the target tag
 * @return the output
 */
template<class E, E Kind>
inline std::ostream& operator<<(std::ostream& out, enum_tag_t<E, Kind> tag) {
    (void) tag;
    return out << Kind;
}

} // namespace util
