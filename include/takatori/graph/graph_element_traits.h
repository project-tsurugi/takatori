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
using graph_element_id_t = decltype(std::declval<T const&>().id());

/// @private
template<class T>
using graph_element_on_join_t = decltype(std::declval<T&>().on_join(std::declval<graph<T>*>(), std::declval<graph_element_id_t<T>>()));

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
        util::is_detected_v<impl::graph_element_id_t, T>
        && util::is_detected_v<impl::graph_element_on_join_t, T>
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

    /// @brief the graph element ID type.
    using id_type = util::detect_t<impl::graph_element_id_t, element_type>;

    /// @brief the graph type.
    using graph_type = graph<element_type>;

    /// @brief the element reference type.
    using reference = std::add_lvalue_reference_t<element_type>;

    /// @brief the const element reference type.
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<element_type>>;

    /**
     * @brief returns the ID of the given element.
     * @param element the target element
     * @return the element ID
     */
    static id_type id(const_reference element);

    /**
     * @brief tells membership information to the given element when the element was join.
     * @param element the target element
     * @param graph the owner
     * @param id the element ID
     */
    static void join(reference element, util::optional_ptr<graph_type> graph, id_type id);

    /**
     * @brief tells when the element was leave from the current graph.
     * If the element has been a member of some graph, this does nothing.
     * @param element the target element
     */
    static void leave(reference element);
};

template<class T>
inline typename graph_element_traits<T>::id_type
graph_element_traits<T>::id(const_reference element) {
    return element.id();
}

template<class T>
inline void
graph_element_traits<T>::join(reference element, util::optional_ptr<graph_type> graph, id_type id) {
    element.on_join(graph.get(), id);
}

template<class T>
inline void
graph_element_traits<T>::leave(reference element) {
    element.on_leave();
}

} // namespace takatori::graph
