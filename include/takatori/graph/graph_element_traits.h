#pragma once

#include "takatori/util/detect.h"
#include "takatori/util/optional_ptr.h"

namespace takatori::graph {

template<class T>
class graph;

/// @private
namespace impl {

/// @private
template<class T>
using graph_element_on_join_t = decltype(std::declval<T&>().on_join(std::declval<graph<T>*>()));

/// @private
template<class T>
using graph_element_on_leave_t = decltype(std::declval<T&>().on_leave());

} // namespace impl

/**
 * @brief provides whether or not the given type satisfies the graph element traits.
 * @tparam T the element type
 */
template<class T>
using is_graph_element = std::bool_constant<
        util::is_detected_v<impl::graph_element_on_join_t, T>
        && util::is_detected_v<impl::graph_element_on_leave_t, T>>;

/// @copydoc is_graph_element
template<class T>
inline constexpr bool is_graph_element_v = is_graph_element<T>::value;

/**
 * @brief traits of graph elements.
 * @tparam T the graph element type.
 */
template<class T>
struct graph_element_traits {

    /// @brief the graph element type.
    using element_type = T;

    /// @brief the graph type.
    using graph_type = graph<element_type>;

    /// @brief the element reference type.
    using reference = std::add_lvalue_reference_t<element_type>;

    /// @brief the const element reference type.
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<element_type>>;

    /**
     * @brief tells membership information to the given element when the element joins.
     * @param element the target element
     * @param graph the owner
     */
    static void join(reference element, util::optional_ptr<graph_type> graph);

    /**
     * @brief tells when the element leaves from the current graph.
     * @details If the element has been a member of some graph, this does nothing.
     * @param element the target element
     */
    static void leave(reference element);
};

template<class T>
inline void
graph_element_traits<T>::join(reference element, util::optional_ptr<graph_type> graph) {
    element.on_join(graph.get());
}

template<class T>
inline void
graph_element_traits<T>::leave(reference element) {
    element.on_leave();
}

} // namespace takatori::graph
