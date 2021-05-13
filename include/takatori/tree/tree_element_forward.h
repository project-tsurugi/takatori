#pragma once

#include <takatori/util/clonable.h>
#include <takatori/util/optional_ptr.h>

namespace takatori::tree {

/**
 * @brief forwards the element for clone objects.
 * @tparam E the element type
 * @tparam D the deleter type
 * @param element the target element
 * @return the forwarded elements
 */
template<class E, class D>
std::unique_ptr<std::remove_const_t<E>> forward(std::unique_ptr<E, D> const& element) {
    return util::clone_unique(element.get());
}

/**
 * @brief forwards the element for clone objects.
 * @tparam E the element type
 * @tparam D the deleter type
 * @param element the target element
 * @return the forwarded elements
 */
template<class E, class D>
std::unique_ptr<E> forward(std::unique_ptr<E, D>&& element) {
    if (!element) {
        return {};
    }
    return util::clone_unique(std::move(element));
}

} // namespace takatori::util
