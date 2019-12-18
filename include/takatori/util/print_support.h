#pragma once

#include <iostream>

#include "detect.h"
#include "pointer_traits.h"

namespace takatori::util {

/// @private
namespace impl {

/// @private
template<class T>
using print_t = decltype(std::declval<std::ostream&>() << std::declval<T>());

} // namespace impl

/**
 * @brief provices whether or not the target type supports printing to std::ostream.
 * @tparam T the target type
 */
template<class T>
using is_printable = std::is_convertible<detect_t<impl::print_t, T>, std::ostream&>;

/// @copydoc is_printable
template<class T>
inline constexpr bool is_printable_v = is_printable<T>::value;

/**
 * @brief printing support for no-printable objects.
 * @tparam T the target type
 */
template<class T, class = void>
class print_support {
public:
    /// @brief the value type
    using value_type = T;

    /**
     * @brief creates a new object.
     * @param object the print target
     */
    explicit constexpr print_support(T const& object) noexcept : ptr_(std::addressof(object)) {}

    /**
     * @brief appends string representation of wrapped object as a pointer.
     * @param out the target output
     * @param object the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, print_support object) {
        return out << object.ptr_;
    }

private:
    T const* ptr_;
};

/**
 * @brief printing support for printable objects.
 * @tparam T the target type
 */
template<class T>
class print_support<
        T,
        std::enable_if_t<is_printable_v<T const&> && !is_pointer_v<T>>> {
public:
    /// @brief the value type
    using value_type = T;

    /**
     * @brief creates a new object.
     * @param object the print target
     */
    explicit constexpr print_support(T const& object) noexcept : ptr_(std::addressof(object)) {}

    /**
     * @brief appends string representation of wrapped object.
     * @param out the target output
     * @param object the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, print_support object) {
        return out << *object.ptr_;
    }

private:
    T const* ptr_;
};

/**
 * @brief printing support for pointer like objects.
 * @tparam Ptr the target type
 */
template<class Ptr>
class print_support<
        Ptr,
        std::enable_if_t<is_printable_v<Ptr const&> && is_pointer_v<Ptr>>> {
public:
    /// @brief the value type
    using value_type = Ptr;

    /**
     * @brief creates a new object.
     * @param object the print target
     */
    explicit constexpr print_support(value_type const& object) noexcept : ptr_(std::addressof(object)) {}

    /**
     * @brief appends string representation of wrapped object.
     * @param out the target output
     * @param object the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, print_support object) {
        using traits = pointer_traits<Ptr>;
        auto&& ref = *object.ptr_;
        if (traits::has_value(ref)) {
            return out << util::print_support { traits::value(ref) };
        }
        return out << "()";
    }

private:
    value_type const* ptr_;
};

/**
 * @brief printing support for boolean values.
 */
template<>
class print_support<bool> {
public:
    /// @brief the value type
    using value_type = bool;

    /**
     * @brief creates a new object.
     * @param value the print target
     */
    explicit constexpr print_support(value_type value) noexcept : value_(value) {}

    /**
     * @brief appends string representation of wrapped object.
     * @param out the target output
     * @param object the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, print_support const& object) {
        if (object.value_) return out << "true";
        return out << "false";
    }

private:
    value_type value_;
};

/// @private
template<class T>
print_support(T const&) -> print_support<T>;

} // namespace takatori::util
