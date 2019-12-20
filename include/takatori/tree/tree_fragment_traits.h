#pragma once

#include <type_traits>

#include <takatori/util/detect.h>
#include <takatori/util/optional_ptr.h>

namespace takatori::tree {

/// @private
namespace impl {

/// @private
template<class T>
struct tree_fragment_parent {
    template<class C, class R, class P>
    static P* capture(R (C::*)(P*)) { return nullptr; }

    template<
            // must be = T
            class U,
            // capture parameter type of T::parent_element(?) - this may detect non-const functions even if T is const
            class P = decltype(capture(&U::parent_element)),
            // validate to call T::parent_element(P&&)
            class = decltype(std::declval<U>().parent_element(std::declval<P>()))>
    static P find(U*) { return nullptr; }

    template<class... Args>
    static util::detect_failure find(Args*...) { return {}; }

    using type = std::remove_pointer_t<decltype(find<T>(nullptr))>;
};

/// @private
template<class T>
struct is_tree_fragment : std::negation<std::is_same<typename tree_fragment_parent<T>::type, util::detect_failure>> {};

} // namespace impl

/**
 * @brief test whether or not the target type satisfies "tree_fragment concept".
 * @tparam T the target type
 */
template<class T> struct is_tree_fragment : impl::is_tree_fragment<T> {};

/// @copydoc is_tree_fragment
template<class T> constexpr inline bool is_tree_fragment_v = is_tree_fragment<T>::value;

/**
 * @brief the traits for tree fragment.
 * @details The tree element "T" must have "T::parent_element(U*) -> void" where "U" is parent_type.
 * @tparam T the element type
 */
template<class T>
struct tree_fragment_traits {

    /// @brief the element type.
    using element_type = T;

    /// @brief the parent element type.
    using parent_type = typename impl::tree_fragment_parent<T>::type;

    /**
     * @brief sets the parent element.
     * @param element the target element
     * @param parent the parent element (may be empty)
     */
    static void set_parent_element(element_type& element, util::optional_ptr<parent_type> parent) {
        element.parent_element(parent.get());
    }
};

} // namespace takatori::tree
