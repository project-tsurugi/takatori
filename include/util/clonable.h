#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "pointer.h"

namespace util {

/// @private
namespace impl {

/// @private
struct is_clonable {
    template<class T>
    std::enable_if_t<
            std::is_base_of_v<T, std::remove_pointer_t<decltype(std::declval<T const&>().clone())>>,
            std::true_type>
    operator()(T&&) const { return {}; }
    std::false_type operator()(...) const { return {}; } // NOLINT
};

} // namespace impl

/**
 * @brief tests if the target type supports polymorphic clone.
 * @tparam T the target type
 */
template<class T> struct is_clonable;

/// @private
template<class T> struct is_clonable : std::invoke_result_t<impl::is_clonable, std::remove_cv_t<std::remove_reference_t<T>>> {};

/// @copydoc is_clonable
template<class T>
constexpr inline bool is_clonable_v = is_clonable<T>::value;

/**
 * @brief provides clone of values.
 * @tparam T the value type
 */
template<class T, class = void> struct cloner;

/**
 * @brief provides clone of values.
 * @tparam T the value type
 */
template<class T>
struct cloner<T, std::enable_if_t<is_clonable_v<T>>> {

    /// @brief the value type
    using value_type = T;

    /// @brief the element type
    using element_type = std::remove_const_t<T>;

    /// @brief the result type
    using result_type = std::unique_ptr<element_type>;

    /**
     * @brief returns a clone of the given value.
     * @param value the target value
     * @return the created clone
     */
    result_type operator()(element_type const& value) { return result_type(value.clone()); }

    /// @copydoc operator()()
    result_type operator()(element_type&& value) { return result_type(std::move(value).clone()); }
};

/**
 * @brief provides clone of values over raw pointer.
 * @tparam T the value type
 */
template<class T>
struct cloner<T*, std::enable_if_t<is_clonable_v<T>>> {

    /// @brief the value type
    using value_type = T*;

    /// @brief the element type
    using element_type = std::remove_const_t<T>;

    /// @brief the result type
    using result_type = std::unique_ptr<element_type>;

    /**
     * @brief returns a clone of the given value.
     * @param value the target value
     * @return the created clone
     */
    std::unique_ptr<element_type> operator()(element_type const* value) {
        return result_type(value == nullptr ? nullptr : value->clone());
    }
};

/**
 * @brief provides clone of values over smart pointer.
 * @tparam T the value type
 * @tparam Container the container type, like std::unique_ptr
 */
template<class T, template<class> class Container>
struct cloner<Container<T>, std::enable_if_t<is_clonable_v<T> && is_smart_pointer_v<Container<T>>>> {

    /// @brief the value type
    using value_type = Container<T>;

    /// @brief the element type
    using element_type = std::remove_const_t<T>;

    /// @brief the result type
    using result_type = std::unique_ptr<std::remove_const_t<T>>;

    /**
     * @brief returns a clone of the given value.
     * @param value the target value
     * @return the created clone
     */
    result_type operator()(std::remove_const_t<value_type> const& value) {
        return result_type(value ? value->clone() : nullptr);
    }

    /// @copydoc operator()()
    result_type operator()(std::remove_const_t<value_type>&& value) {
        return result_type(value ? std::move(value)->clone() : nullptr);
    }
};

/**
 * @brief returns a clone of the input.
 * @tparam T the input type
 * @param value the target value, one of clonable references, raw pointers, or smart pointers
 * @return the created clone
 */
template<class T>
constexpr inline typename cloner<std::remove_const_t<std::remove_reference_t<T>>>::result_type clone(T&& value) {
    return cloner<std::remove_const_t<std::remove_reference_t<T>>>{}(std::forward<T>(value));
}

} // namespace util
