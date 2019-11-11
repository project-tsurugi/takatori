#pragma once

#include "takatori/tree/tree_element_vector.h"

#include "takatori/util/clonable.h"
#include "takatori/util/object_creator.h"

namespace takatori::tree {

/**
 * @brief forward elements for clone objects.
 * @tparam E the element type
 * @tparam C the copier type
 * @param creator the object creator
 * @param elements the target elements
 * @return the forwarded elements
 */
template<class E, class C = util::null_copier<E>>
util::reference_vector<E, C>
forward(util::object_creator creator, tree_element_vector<E> const& elements) {
    util::reference_vector<E, C> copy { creator };
    copy.reserve(elements.size());
    for (auto&& e : elements) {
        copy.unsafe_push_back(util::clone(e, creator));
    }
    return copy;
}

/**
 * @brief forward elements for clone objects.
 * @tparam E the element type
 * @tparam C the copier type
 * @param creator the object creator
 * @param elements the target elements
 * @return the forwarded elements
 */
template<class E, class C = util::null_copier<E>>
util::reference_vector<E, C>
forward(util::object_creator creator, tree_element_vector<E>&& elements) {
    if (creator == elements.get_object_creator()) {
        // FIXME: deep clone?
        return elements.release_elements();
    }
    util::reference_vector<E, C> copy { creator };
    copy.reserve(elements.size());
    for (auto&& e : elements) {
        copy.unsafe_push_back(util::clone(std::move(e), creator));
    }
    return copy;
}

} // namespace takatori::util
