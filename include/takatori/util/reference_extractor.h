#pragma once

#include <type_traits>
#include <utility>

namespace takatori::util {

/**
 * @brief extracts reference from pointer.
 * @tparam T the value type
 */
template<class T>
struct pointer_extractor {
    /// @brief the value type
    using value_type = T;
    /// @brief the difference type
    using difference_type = std::ptrdiff_t;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<value_type>;
    /// @brief the reference type.
    using reference = std::add_lvalue_reference_t<value_type>;
    /// @brief the cursor type.
    using cursor_type = pointer;

    /**
     * @brief extracts a reference on the cursor.
     * @param cursor the target cursor
     * @return the reference
     */
    static constexpr reference get(cursor_type cursor) noexcept { return *cursor; }

    /**
     * @brief advances the cursor
     * @param cursor the target cursor
     * @param offset the cursor offset
     * @return the advanced cursor
     */
    static constexpr cursor_type advance(cursor_type cursor, difference_type offset) noexcept { return cursor + offset; } // NOLINT
};

/**
 * @brief extracts reference from pointer of pointer.
 * @tparam T the value type
 */
template<class T>
struct double_pointer_extractor {
    /// @brief the value type
    using value_type = T;
    /// @brief the difference type
    using difference_type = std::ptrdiff_t;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<value_type>;
    /// @brief the reference type.
    using reference = std::add_lvalue_reference_t<value_type>;
    /// @brief the cursor type.
    using cursor_type = std::add_pointer_t<std::add_const_t<pointer>>;

    /**
     * @brief extracts a reference on the cursor.
     * @param cursor the target cursor
     * @return the reference
     */
    static constexpr reference get(cursor_type cursor) noexcept { return **cursor; }

    /**
     * @brief advances the cursor
     * @param cursor the target cursor
     * @param offset the cursor offset
     * @return the advanced cursor
     */
    static constexpr cursor_type advance(cursor_type cursor, difference_type offset) noexcept { return cursor + offset; } // NOLINT
};

} // namespace takatori::util
