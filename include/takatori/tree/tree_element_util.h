#pragma once

#include <takatori/util/ownership_reference.h>

#include "tree_fragment_traits.h"

namespace takatori::tree {

/**
 * @brief set parent to the element.
 * @tparam Parent the parent element type
 * @tparam E the element type
 * @param parent the parent element
 * @param element the target element
 * @return the element
 */
template<class Parent, class E>
E&& bless_element(Parent&& parent, E&& element) {
    static_assert(is_tree_fragment_v<std::remove_reference_t<E>>);
    using traits = tree_fragment_traits<std::remove_reference_t<E>>;
    traits::set_parent_element(element, util::optional_ptr<typename traits::parent_type> { parent });
    return std::forward<E>(element);
}

/**
 * @brief set parent to the element.
 * @tparam Parent the parent element type
 * @tparam E the element type
 * @tparam D the deleter type
 * @param parent the parent element
 * @param element the target element
 * @return the element
 */
template<class Parent, class E, class D>
std::unique_ptr<E, D>& bless_element(Parent&& parent, std::unique_ptr<E, D>& element) {
    if (element) {
        bless_element(std::forward<Parent>(parent), *element);
    }
    return element;
}

/**
 * @brief set parent to the element.
 * @tparam Parent the parent element type
 * @tparam E the element type
 * @tparam D the deleter type
 * @param parent the parent element
 * @param element the target element
 * @return the element
 */
template<class Parent, class E, class D>
std::unique_ptr<E, D> bless_element(Parent&& parent, std::unique_ptr<E, D>&& element) {
    if (element) {
        bless_element(std::forward<Parent>(parent), *element);
    }
    return std::move(element);
}

/**
 * @brief removes parent information.
 * @tparam E the element type
 * @param element the element container
 * @return the element
 */
template<class E>
E&& unbless_element(E&& element) {
    static_assert(is_tree_fragment_v<std::remove_reference_t<E>>);
    using traits = tree_fragment_traits<std::remove_reference_t<E>>;
    traits::set_parent_element(element, nullptr);
    return std::forward<E>(element);
}

/**
 * @brief releases element and remove its parent information.
 * @tparam E the element type
 * @tparam D the deleter type
 * @param element the element container
 * @return the released element container
 */
template<class E, class D>
std::unique_ptr<E, D> release_element(std::unique_ptr<E, D>&& element) {
    if (element) {
        unbless_element(*element);
    }
    return std::move(element);
}

/**
 * @brief assigns an element into the container.
 * @tparam Parent the parent element type
 * @tparam E the element type
 * @tparam D the deleter type
 * @param parent the parent element
 * @param destination the destination container
 * @param source the source container
 * @return the parent element
 */
template<class Parent, class E, class D>
Parent& assign_element(Parent& parent, std::unique_ptr<E, D>& destination, std::unique_ptr<E, D>&& source) {
    static_assert(is_tree_fragment_v<E>);
    using traits = tree_fragment_traits<E>;
    static_assert(std::is_convertible_v<Parent*, typename traits::parent_type*>);
    destination = bless_element(parent, std::move(source));
    return parent;
}

/**
 * @brief assigns an element into the container.
 * @tparam Parent the parent element type
 * @tparam E the element type
 * @tparam D the deleter type
 * @param parent the parent element
 * @param destination the destination container
 * @param source the source container
 */
template<class Parent, class E, class D>
void assign_element_fragment(Parent* parent, std::unique_ptr<E, D>& destination, std::unique_ptr<E, D>&& source) {
    static_assert(is_tree_fragment_v<E>);
    using traits = tree_fragment_traits<E>;
    static_assert(std::is_convertible_v<Parent*, typename traits::parent_type*>);
    if (parent != nullptr) {
        destination = bless_element(*parent, std::move(source));
    } else {
        destination = release_element(std::move(source));
    }
}

/**
 * @brief returns the ownership reference of the given property.
 * @tparam Parent the parent element type
 * @tparam E the element type
 * @tparam D the deleter type
 * @param parent the parent element
 * @param property the property container reference
 * @return the corresponded ownership reference
 */
template<class Parent, class E, class D>
util::ownership_reference<E, D> ownership_element(Parent& parent, std::unique_ptr<E, D>& property) {
    static_assert(is_tree_fragment_v<E>);
    using traits = tree_fragment_traits<E>;
    static_assert(std::is_convertible_v<Parent*, typename traits::parent_type*>);
    return util::ownership_reference<E, D> {
            [&]() {
                return property.get();
            },
            [&](std::unique_ptr<E, D> replacement) {
                assign_element(parent, property, std::move(replacement));
            }
    };
}

/**
 * @brief returns the ownership reference of the given property.
 * @tparam Parent the parent element type
 * @tparam E the element type
 * @tparam D the deleter type
 * @param parent the parent element
 * @param property the property container reference
 * @return the corresponded ownership reference
 */
template<class Parent, class E, class D>
util::ownership_reference<E, D> ownership_element_fragment(Parent*& parent, std::unique_ptr<E, D>& property) {
    static_assert(is_tree_fragment_v<E>);
    using traits = tree_fragment_traits<E>;
    static_assert(std::is_convertible_v<Parent*, typename traits::parent_type*>);
    return util::ownership_reference<E, D> {
            [&]() {
                return property.get();
            },
            [&](std::unique_ptr<E, D> replacement) {
                assign_element_fragment(parent, property, std::move(replacement));
            }
    };
}

} // namespace takatori::tree
