#pragma once

#include <memory>
#include <type_traits>

namespace takatori::util {

/**
 * @brief an optional R-value reference.
 * @tparam T the value type
 */
template<class T>
class rvalue_ptr {

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

    /**
     * @brief creates a new instance.
     */
    rvalue_ptr() = default;

    /**
     * @brief creates a new instance.
     */
    rvalue_ptr(std::nullptr_t) noexcept {} // NOLINT

    /**
     * @brief creates a new instance.
     * @param entry the target reference
     */
    rvalue_ptr(std::remove_reference_t<T>&& entry) noexcept : entry_(std::addressof(entry)) {} // NOLINT

    /**
     * @brief creates a new instance.
     * @param ptr the target pointer
     */
    explicit rvalue_ptr(T* ptr) noexcept : entry_(ptr) {} // NOLINT

    /**
     * @brief creates a new instance.
     * @tparam U the target value type
     * @param other the copy source
     */
    template<
            class U,
            class = std::enable_if_t<std::is_constructible_v<pointer, typename rvalue_ptr<U>::pointer>>>
    rvalue_ptr(rvalue_ptr<U> other) noexcept : entry_(other.entry_) {} // NOLINT

    /**
     * @brief returns whether or not this reference is empty.
     * @return true if this reference is empty
     * @return false otherwise
     */
    constexpr bool empty() const noexcept { return entry_ == nullptr; }

    /**
     * @brief returns whether or not this has any entries.
     * @return true if this has entry
     * @return false otherwise
     */
    constexpr bool has_value() const noexcept { return !empty(); }

    /**
     * @brief returns a pointer to the holding value.
     * @return pointer to the holding value
     * @return nullptr if this reference is empty
     */
    constexpr pointer get() const noexcept { return entry_; }

    /**
     * @brief returns the holding value.
     * @return the holding value
     * @warning undefined behavior if the reference is empty
     * @see get()
     */
    constexpr reference value() { return std::move(*entry_); }

    /// @copydoc has_value()
    explicit constexpr operator bool() const noexcept { return has_value(); }

    /// @copydoc value()
    constexpr reference operator*() const noexcept { return value(); }

    /// @copydoc get()
    constexpr pointer operator->() const noexcept { return get(); }

private:
    pointer entry_ {};

    template<class U>
    friend class rvalue_ptr;
};

/// @private
template<class T> rvalue_ptr(T&&) -> rvalue_ptr<std::remove_reference_t<T>>;

/// @private
template<class T> rvalue_ptr(T*) -> rvalue_ptr<T>;

} // namespace takatori::util
