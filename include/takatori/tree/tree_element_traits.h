#pragma once

#include <memory>
#include <type_traits>

#include "takatori/util/detect.h"
#include "takatori/util/meta_type.h"
#include "takatori/util/optional_ptr.h"

namespace takatori::tree {

/// @private
namespace impl {

/// @private
template<class T>
using tree_element_parent_t = decltype(std::declval<T>().parent_element());

/// @private
template<class T>
using is_tree_element = std::is_pointer<util::detect_t<tree_element_parent_t, T const>>;

/// @private
template<class T>
using tree_element_parent = std::conditional<
        is_tree_element<T>::value,
        std::remove_cv_t<std::remove_pointer_t<util::detect_t<tree_element_parent_t, T const>>>,
        void>;

/// @private
template<class T>
using tree_element_can_get_mutable_parent = std::is_same<
        std::remove_pointer_t<util::detect_t<tree_element_parent_t, T>>,
        typename tree_element_parent<T>::type>;

} // namespace impl

/**
 * @brief test whether or not the target type satisfies "tree_element concept".
 * @tparam T the target type
 */
template<class T> struct is_tree_element : impl::is_tree_element<T> {};

/// @copydoc is_tree_element
template<class T> constexpr inline bool is_tree_element_v = is_tree_element<T>::value;

/**
 * @brief the traits for tree_element.
 * @details The tree element "T" must have "T::parent_element() const -> U const*" where "U" is parent_type.
 * Additionally, tree elements may have the following optional features.
 *   @li 1. "T::parent_element() -> U*" - can_get_mutable_parent
 * @tparam T the element type
 */
template<class T>
struct tree_element_traits {

    /// @brief the element type.
    using element_type = T;

    /// @brief the parent element type.
    using parent_type = typename impl::tree_element_parent<T>::type;

    /// @brief whether or not the element provides a mutable parent element
    constexpr static inline bool can_get_mutable_parent = impl::tree_element_can_get_mutable_parent<T>::value;

    /**
     * @brief returns the parent element.
     * @param element the target element
     * @return the parent element
     * @return empty if the element does not have its parent
     */
    static util::optional_ptr<parent_type const> get_parent_element(std::remove_const_t<element_type> const& element) noexcept {
        static_assert(is_tree_element_v<T>);
        return util::optional_ptr { element.parent_element() };
    }

    /// @copydoc get_parent_element()
    static util::optional_ptr<parent_type> get_parent_element(std::remove_const_t<element_type>& element) noexcept {
        static_assert(is_tree_element_v<T>);
        static_assert(can_get_mutable_parent);
        return util::optional_ptr { element.parent_element() };
    }
};

} // namespace takatori::tree
