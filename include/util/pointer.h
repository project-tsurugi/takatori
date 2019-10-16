#pragma once

#include <type_traits>

#include "util/meta_type.h"

namespace util {

/// @private
namespace impl {

/// @private
template<class T> struct is_bool_support : std::is_constructible<bool, T> {};

/// @private
template<class T, class = void> struct is_dereference_support : std::false_type {};

/// @private
template<class T, template<class> class C> struct is_dereference_support<
        C<T>,
        std::enable_if_t<std::is_same_v<std::add_lvalue_reference_t<T>, decltype(std::declval<C<T>&>().operator*())>>>
    : std::true_type
{};

/// @private
template<class T, class = void> struct is_arrow_support : std::false_type {};

/// @private
template<class T, template<class> class C> struct is_arrow_support<
        C<T>,
        std::enable_if_t<std::is_same_v<std::add_pointer_t<T>, decltype(std::declval<C<T>&>().operator->())>>>
    : std::true_type
{};

/// @private
template<class T, class = void>
struct smart_pointer_helper : std::false_type {
    using remove_smart_pointer = T;
};

/// @private
template<class T, template<class> class Container>
struct smart_pointer_helper<
        Container<T>,
        std::enable_if_t<std::conjunction_v<
                is_bool_support<Container<T>>,
                is_dereference_support<Container<T>>,
                is_arrow_support<Container<T>>>>>
        : std::true_type {
    using remove_smart_pointer = T;
};

} // namespace impl

/**
 * @brief tests if the target type is a raw pointer.
 * @tparam T the target type
 */
template<class T> using is_raw_pointer = std::is_pointer<T>;

/**
 * @brief tests if the target type is a smart pointer.
 * That is, the target type supports the all following features:
 * - bool conversion operator : returns true iff not empty
 * - de-reference operator : returns reference of the pointing data
 * - arrow operator : returns raw pointer of the pointing data
 * @tparam T the target type
 */
template<class T> struct is_smart_pointer : impl::smart_pointer_helper<T> {};

/**
 * @brief provides the value type of the smart pointer.
 * If the specified type is not a smart pointer, this just returns the specified type.
 * @tparam T the target type
 */
template<class T> struct remove_smart_pointer
        : meta_type<typename impl::smart_pointer_helper<T>::remove_smart_pointer> {};

/// @copydoc is_raw_pointer
template<class T> constexpr inline bool is_raw_pointer_v = std::is_pointer_v<T>;

/// @copydoc is_smart_pointer
template<class T> constexpr inline bool is_smart_pointer_v = is_smart_pointer<T>::value;

/// @copydoc remove_smart_pointer
template<class T> using remove_smart_pointer_t = typename remove_smart_pointer<T>::type;

} // namespace util
