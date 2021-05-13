#pragma once

#include <takatori/tree/tree_element_vector.h>

#include <takatori/util/clonable.h>

namespace takatori::tree {

/**
 * @brief forward elements for clone objects.
 * @tparam E the element type
 * @tparam C the copier type
 * @param elements the target elements
 * @return the forwarded elements
 */
template<class E, class C = util::null_copier<E>>
util::reference_vector<E, C>
forward(tree_element_vector<E> const& elements) {
    util::reference_vector<E, C> copy {};
    copy.reserve(elements.size());
    for (auto&& e : elements) {
        copy.unsafe_push_back(util::clone(e));
    }
    return copy;
}

/**
 * @brief forward elements for clone objects.
 * @tparam E the element type
 * @tparam C the copier type
 * @param elements the target elements
 * @return the forwarded elements
 */
template<class E, class C = util::null_copier<E>>
util::reference_vector<E, C>
forward(tree_element_vector<E>&& elements) {
    // FIXME: deep clone?
    return elements.release_elements();
}

} // namespace takatori::util
