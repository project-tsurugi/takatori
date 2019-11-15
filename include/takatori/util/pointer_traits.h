#pragma once

#include <type_traits>

#include "meta_type.h"

namespace takatori::util {

/// @private
namespace impl {

/// @private
template<class T> struct is_bool_support : std::is_constructible<bool, T> {};

/// @private
template<class T, class = void> struct is_dereference_support : std::is_pointer<T> {};

// FIXME: detector idiom

/// @private
template<class Ptr> struct is_dereference_support<
        Ptr,
        std::enable_if_t<std::is_same_v<
                std::add_lvalue_reference_t<typename Ptr::element_type>,
                decltype(std::declval<Ptr&>().operator*())>>>
    : std::true_type
{};

/// @private
template<class T, class = void> struct is_arrow_support : std::is_pointer<T> {};

/// @private
template<class Ptr> struct is_arrow_support<
        Ptr,
        std::enable_if_t<std::is_same_v<
                std::add_pointer_t<typename Ptr::element_type>,
                decltype(std::declval<Ptr&>().operator->())>>>
    : std::true_type
{};

/// @private
template<class T, class = void> struct is_nullptr_assignment_support : std::is_pointer<T> {};

/// @private
template<class Ptr> struct is_nullptr_assignment_support<
        Ptr,
        std::enable_if_t<std::is_assignable_v<Ptr&, std::nullptr_t>>>
    : std::true_type
{};

/// @private
template<class T> struct is_pointer : std::conjunction<
        impl::is_bool_support<T>,
        impl::is_dereference_support<T>,
        impl::is_arrow_support<T>> {};

/// @private
template<class T> struct is_smart_pointer : std::conjunction<
        std::negation<std::is_pointer<T>>,
        is_pointer<T>> {};

} // namespace impl

/**
 * @brief tests if the target type is supports the "pointer" concept.
 * That is, the target type supports the all following features:
 * - bool conversion operator : returns true iff not empty
 * - de-reference operator : returns reference of the pointing data
 * - arrow operator : returns raw pointer of the pointing data
 * @tparam T the target type
 */
template<class T> struct is_pointer : impl::is_pointer<T> {};

/// @copydoc is_pointer
template<class T> constexpr inline bool is_pointer_v = is_pointer<T>::value;

/**
 * @brief tests if the target type is a raw pointer.
 * @tparam T the target type
 */
template<class T> using is_raw_pointer = std::is_pointer<T>;

/// @copydoc is_raw_pointer
template<class T> constexpr inline bool is_raw_pointer_v = is_raw_pointer<T>::value;

/**
 * @brief tests if the target type is a smart pointer.
 * @tparam T the target type
 */
template<class T> struct is_smart_pointer : impl::is_smart_pointer<T> {};

/// @copydoc is_smart_pointer
template<class T> constexpr inline bool is_smart_pointer_v = is_smart_pointer<T>::value;

/**
 * @brief provides pointer traits of the target type.
 * @tparam T the target type
 */
template<class T, class = void>
struct pointer_traits {};

/**
 * @brief provides pointer traits for a raw pointer type.
 * @tparam E the element type
 */
template<class E>
struct pointer_traits<E*, std::enable_if_t<is_pointer_v<E*>>> {

    /// @brief the original pointer type
    using type = E*;

    /// @brief the element type
    using element_type = std::remove_pointer_t<type>;

    /// @brief the L-value reference of the element
    using reference = std::add_lvalue_reference_t<element_type>;

    /// @brief the const L-value reference of the element
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<element_type>>;

    /// @brief the R-value reference of the element
    using rvalue_reference = std::add_rvalue_reference_t<element_type>;

    /// @brief the raw pointer to the element
    using pointer = std::add_pointer_t<element_type>;

    /// @brief the raw pointer to the element
    using const_pointer = std::add_pointer_t<std::add_const_t<element_type>>;

    /**
     * @brief returns whether or not the pointer has value.
     * @param value the target pointer
     * @return true if the pointer is not empty
     * @return false otherwise
     */
    static constexpr bool has_value(E* value) noexcept { return static_cast<bool>(value); }

    /**
     * @brief returns the reference of the target pointer.
     * @param value the target pointer
     * @return the reference
     * @return undefined if the pointer is null
     */
    static constexpr reference value(E* value) noexcept { return *value; }

    /**
     * @brief removes the pointed value.
     * After this operator, has_value(value) will return nullptr.
     * @param value the value
     */
    static constexpr void reset(E*& value) noexcept { value = nullptr; }
};

/**
 * @brief provides pointer traits for a smart pointer type.
 * @tparam Ptr the smart pointer type
 * @tparam Container the container type
 */
template<class Ptr>
struct pointer_traits<Ptr, std::enable_if_t<is_smart_pointer_v<Ptr>>> {

    /// @brief the original pointer type
    using type = Ptr;

    /// @brief the element type
    using element_type = typename type::element_type;

    /// @brief the L-value reference of the element
    using reference = std::add_lvalue_reference_t<element_type>;

    /// @brief the const L-value reference of the element
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<element_type>>;

    /// @brief the R-value reference of the element
    using rvalue_reference = std::add_rvalue_reference_t<element_type>;

    /// @brief the raw pointer to the element
    using pointer = std::add_pointer_t<element_type>;

    /// @brief the raw pointer to the element
    using const_pointer = std::add_pointer_t<std::add_const_t<element_type>>;

    /**
     * @brief returns whether or not the pointer has value.
     * @param value the target pointer
     * @return true if the pointer is not empty
     * @return false otherwise
     */
    static constexpr bool has_value(Ptr const& value) noexcept { return static_cast<bool>(value); }

    /**
     * @brief returns the reference of the target pointer.
     * @param value the target pointer
     * @return the reference
     * @return undefined if the pointer is null
     */
    static constexpr reference value(Ptr const& value) noexcept { return *value; }

    /**
     * @brief removes the pointed value.
     * After this operation, has_value(value) will return nullptr.
     * @param value the value
     */
    static constexpr void reset(Ptr& value) noexcept { value = nullptr; }
};

/// @private
namespace impl {

/// @private
template<class T, class = void>
struct remove_pointer : meta_type<T> {};

/// @private
template<class T>
struct remove_pointer<T, std::enable_if_t<is_pointer_v<T>>> : meta_type<typename pointer_traits<T>::element_type> {};

} // namespace impl

/**
 * @brief provides the value type of the smart pointer.
 * If the specified type is not a smart pointer, this just returns the specified type.
 * @tparam T the target type
 */
template<class T> struct remove_pointer : impl::remove_pointer<T> {};

/// @copydoc remove_pointer
template<class T> using remove_pointer_t = typename remove_pointer<T>::type;

} // namespace takatori::util
