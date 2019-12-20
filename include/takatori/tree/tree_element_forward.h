#pragma once

#include <takatori/util/clonable.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/optional_ptr.h>

namespace takatori::tree {

/**
 * @brief forwards the element for clone objects.
 * @tparam E the element type
 * @tparam D the deleter type
 * @param creator the object creator
 * @param element the target element
 * @return the forwarded elements
 */
template<class E, class D>
util::unique_object_ptr<std::remove_const_t<E>> forward(util::object_creator creator, std::unique_ptr<E, D> const& element) {
    return util::clone_unique(element.get(), creator);
}

/**
 * @brief forwards the element for clone objects.
 * @tparam E the element type
 * @tparam D the deleter type
 * @param creator the object creator
 * @param element the target element
 * @return the forwarded elements
 */
template<class E, class D>
util::unique_object_ptr<E> forward(util::object_creator creator, std::unique_ptr<E, D>&& element) {
    if (!element) {
        return { nullptr, util::object_deleter(creator) };
    }
    return util::clone_unique(std::move(element), creator);
}

} // namespace takatori::util
