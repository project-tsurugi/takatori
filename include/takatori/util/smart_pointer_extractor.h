#pragma once

#include <type_traits>
#include <utility>

#include "pointer_traits.h"

namespace takatori::util {

/**
 * @brief extracts references from smart pointers.
 * @tparam Ptr the smart pointer type
 */
template<class Ptr>
struct smart_pointer_extractor {
    /// @brief the smart pointer type.
    using smart_pointer = Ptr;
    /// @brief the value type
    using value_type = typename pointer_traits<smart_pointer>::element_type;
    /// @brief the difference type
    using difference_type = std::ptrdiff_t;
    /// @brief the pointer type.
    using pointer = typename pointer_traits<smart_pointer>::pointer;
    /// @brief the reference type.
    using reference = typename pointer_traits<smart_pointer>::reference;
    /// @brief the cursor type.
    using cursor_type = std::add_pointer_t<std::add_const_t<smart_pointer>>;

    /**
     * @brief extracts a reference on the cursor.
     * @param cursor the target cursor
     * @return the reference
     */
    [[nodiscard]] constexpr reference get(cursor_type cursor) const noexcept {
        return pointer_traits<smart_pointer>::value(*cursor);
    }

    /**
     * @brief advances the cursor
     * @param cursor the target cursor
     * @param offset the cursor offset
     * @return the advanced cursor
     */
    [[nodiscard]] constexpr cursor_type advance(cursor_type cursor, difference_type offset) const noexcept {
        return cursor + offset; // NOLINT
    }

    /**
     * @brief returns the distance of two cursors.
     * @param from the base cursor
     * @param to the target cursor
     * @return the distance from the base cursor to the target cursor
     */
    [[nodiscard]] constexpr difference_type distance(cursor_type from, cursor_type to) const noexcept {
        return to - from; // NOLINT
    }
};

} // namespace takatori::util
