#pragma once

#include <type_traits>
#include <utility>

namespace takatori::util {

/**
 * @brief a wrapper class of R-value references.
 * @details This is designed to pass non-copyable values into std::initializer_list.
 *      You can declare `std::initializer_list<rvalue_reference_wrapper<T>>` parameter,
 *      and then you can pass rvalue references as an initializer list.
 *      To build a `std::vector<T>` (or similar collection) from the above parameter,
 *      you can simply pass the iterator range of the initializer list to the vector's constructor.
 *       It behave as like std::move_iterator by implicit conversion operator of rvalue_reference_wrapper which
 *       provides rvalue reference of the component.
 * @tparam T the value type
 */
template<class T>
class rvalue_reference_wrapper {

    static_assert(!std::is_reference_v<T>, "value must not be a reference type");

public:
    /// @brief the value type.
    using value_type = T;
    /// @brief the element type.
    using element_type = T;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<T>;
    /// @brief the reference type.
    using reference = std::add_rvalue_reference_t<T>;

    rvalue_reference_wrapper() = delete;

    /**
     * @brief creates a new instance.
     * @param entry the target reference
     */
    rvalue_reference_wrapper(std::remove_const_t<T>&& entry) noexcept : entry_(std::addressof(entry)) {} // NOLINT

    /**
     * @brief creates a new instance.
     * @tparam U the target value type
     * @param other the copy source
     */
    template<
            class U,
            class = std::enable_if_t<std::is_constructible_v<pointer, typename rvalue_reference_wrapper<U>::pointer>>>
    rvalue_reference_wrapper(rvalue_reference_wrapper<U> other) noexcept : entry_(other.entry_) {} // NOLINT

    /**
     * @brief returns the holding value.
     * @return the holding value
     */
    [[nodiscard]] constexpr reference get() const noexcept { return std::move(*entry_); }

    /// @copydoc get()
    [[nodiscard]] constexpr operator T&&() const noexcept { return get(); } // NOLINT

private:
    pointer entry_;

    template<class U>
    friend class rvalue_reference_wrapper;
};

/// @private
template<class T> rvalue_reference_wrapper(T&&) -> rvalue_reference_wrapper<std::remove_reference_t<T>>;

/**
 * @brief creates a new rvalue_reference_wrapper object.
 * @tparam T the value type
 * @param value reference to the target value
 * @return the wrapped reference
 */
template<class T>
inline std::enable_if_t<
        std::is_rvalue_reference_v<T&&>,
        rvalue_reference_wrapper<std::remove_reference_t<T>>>
rvref(T&& value) noexcept {
    return rvalue_reference_wrapper<std::remove_reference_t<T>>(std::forward<T>(value));
}

} // namespace takatori::util

