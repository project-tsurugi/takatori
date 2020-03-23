#pragma once

namespace takatori::util {

/**
 * @brief a tag to distinguish which the operation means "post visit".
 */
struct post_visit {

    /**
     * @brief creates a new instance.
     */
    explicit constexpr post_visit() = default;
};

} // namespace takatori::util
