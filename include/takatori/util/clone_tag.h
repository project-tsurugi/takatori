#pragma once

#include <type_traits>

namespace takatori::util {

/**
 * @brief tag type for constructor which constructs a copy of the target object.
 */
struct clone_tag_t {
    /// @brief the explicit constructor.
    constexpr explicit clone_tag_t() noexcept = default;
};

/**
 * @brief tag for constructor which constructs a copy of the target object.
 */
static constexpr clone_tag_t clone_tag {};

/**
 * @brief whether or not the target type is clone constructible.
 * @tparam T the target type
 */
template<class T>
using is_clone_constructible = std::is_constructible<T, clone_tag_t, T const&>;

/// @copydoc is_clone_constructible
template<class T>
static constexpr bool is_clone_constructible_v = is_clone_constructible<T>::value; // NOLINT(modernize-type-traits)

} // namespace takatori::util
