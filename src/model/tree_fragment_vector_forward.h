#pragma once

#include "model/tree_fragment_vector.h"

#include "util/clonable.h"
#include "util/object_creator.h"

namespace model {

/**
 * @brief forward fragments for clone objects.
 * @tparam E the element type
 * @param creator the object creator
 * @param elements the target fragments
 * @return the forwarded fragments
 */
template<class E>
std::vector<E, util::pmr::polymorphic_allocator<E>>
forward(util::object_creator creator, tree_fragment_vector<E> const& elements) {
    static_assert(
            std::is_copy_constructible_v<E> || util::is_copy_constructible_with_object_creator_v<E>,
            "fragment must be either copy constructible with or without object_creator");
    std::vector<E, util::pmr::polymorphic_allocator<E>> copy { creator.allocator<E>() };
    copy.reserve(elements.size());
    for (auto&& e : elements) {
        if constexpr (util::is_copy_constructible_with_object_creator_v<E>) { // NOLINT
            copy.emplace_back(e, creator);
        } else if constexpr (std::is_copy_constructible_v<E>) { // NOLINT
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
 * @param creator the object creator
 * @param elements the target fragments
 * @return the forwarded fragments
 */
template<class E>
std::vector<E, util::pmr::polymorphic_allocator<E>>
forward(util::object_creator creator, tree_fragment_vector<E>&& elements) {
    static_assert(
            std::is_move_constructible_v<E> || util::is_move_constructible_with_object_creator_v<E>,
            "fragment must be either move constructible with or without object_creator");
    if (creator == elements.get_object_creator()) {
        // FIXME: deep clone?
        return elements.release_elements();
    }
    if constexpr (util::is_move_constructible_with_object_creator_v<E>) { // NOLINT
        std::vector<E, util::pmr::polymorphic_allocator<E>> copy { creator.allocator<E>() };
        copy.reserve(elements.size());
        for (auto&& e : elements.release_elements()) {
            copy.emplace_back(std::move(e), creator);
        }
        return copy;
    }
    if constexpr (std::is_move_constructible_v<E>) { // NOLINT
        return { elements.release_elements(), creator.allocator<E>() };
    }
    std::abort();
}

} // namespace model
