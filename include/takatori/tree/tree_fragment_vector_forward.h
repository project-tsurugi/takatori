#pragma once

#include <takatori/tree/tree_fragment_vector.h>

#include <takatori/util/clonable.h>
#include <takatori/util/clone_tag.h>

namespace takatori::tree {

/**
 * @brief forward fragments for clone objects.
 * @tparam E the element type
 * @param elements the target fragments
 * @return the forwarded fragments
 */
template<class E>
std::vector<E>
forward(tree_fragment_vector<E> const& elements) {
    static_assert(
            std::is_copy_constructible_v<E> || util::is_clone_constructible_v<E>,
            "fragment must be either copy constructible with takatori::util::clone_tag");
    std::vector<E> copy {};
    copy.reserve(elements.size());
    for (auto&& e : elements) {
        if constexpr (util::is_clone_constructible_v<E>) {
            copy.emplace_back(util::clone_tag, e);
        } else if constexpr (std::is_copy_constructible_v<E>) {
            copy.emplace_back(e);
        } else {
            std::abort();
        }
    }
    return copy;
}

/**
 * @brief forward fragments for clone objects.
 * @tparam E the element type
 * @param elements the target fragments
 * @return the forwarded fragments
 */
template<class E>
std::vector<E>
forward(tree_fragment_vector<E>&& elements) {
    // FIXME: deep clone?
    return elements.release_elements();
}

} // namespace takatori::util
