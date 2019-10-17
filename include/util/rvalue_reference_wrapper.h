#pragma once

#include <type_traits>
#include <utility>

namespace util {

/**
 * @brief a wrapper class of R-value references.
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
    constexpr reference get() const noexcept { return std::move(*entry_); }

    /// @copydoc get()
    constexpr operator T&&() const noexcept { return get(); } // NOLINT

private:
    pointer entry_;

    template<class U>
    friend class rvalue_reference_wrapper;
};

/// @private
template<class T> rvalue_reference_wrapper(T&&) -> rvalue_reference_wrapper<std::remove_reference_t<T>>;

template<class T>
inline std::enable_if_t<
        !std::is_rvalue_reference_v<T>,
        rvalue_reference_wrapper<std::remove_reference_t<T>>>
rvref(T&& value) noexcept {
    return rvalue_reference_wrapper<std::remove_reference_t<T>>(std::forward<T>(value));
}

} // namespace util

