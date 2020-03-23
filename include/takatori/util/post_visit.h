#pragma once

namespace takatori::util {

/**
 * @brief a tag to distinguish which the operation means "post visit".
 */
struct post_visit_t {

    /**
     * @brief creates a new instance.
     */
    explicit constexpr post_visit_t() = default;
};

/// @brief an instance of post_visit_t.
inline constexpr post_visit_t post_visit;

} // namespace takatori::util
