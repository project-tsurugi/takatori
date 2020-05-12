#pragma once

#include <cstdint>

#include "port_direction.h"

#include <takatori/util/exception.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/sequence_view.h>

namespace takatori::graph {

template<class Vertex>
class graph;

/**
 * @brief an abstract implementation of vertex of graph.
 * @tparam Vertex the actual vertex type
 * @tparam Port the port type template
 */
template<class Vertex, template <class, port_direction> class Port>
class vertex_base {
public:
    /// @brief the vertex type
    using vertex_type = Vertex;

    /// @brief the graph type
    using graph_type = graph<vertex_type>;

    /// @brief the input port type.
    using input_port_type = Port<vertex_type, port_direction::input>;

    /// @brief the output port type.
    using output_port_type = Port<vertex_type, port_direction::output>;

    virtual ~vertex_base() = default;
    vertex_base(vertex_base const& other) = delete;
    vertex_base& operator=(vertex_base const& other) = delete;
    vertex_base(vertex_base&& other) noexcept = delete;
    vertex_base& operator=(vertex_base&& other) noexcept = delete;

    /**
     * @brief returns whether or not this vertex is orphaned.
     * @return true if this vertex does not have the owner graph
     * @return false otherwise
     */
    bool is_orphan() const noexcept { return !optional_owner(); }

    /**
     * @brief returns the graph which owns this vertex.
     * @return the owner
     * @throws std::domain_error if this vertex is orphaned
     * @see is_orphaned()
     * @see optional_owner()
     */
    [[nodiscard]] graph_type& owner() {
        if (auto g = optional_owner()) return *g;
        util::throw_exception(std::domain_error("the vertex is orphaned"));
    }

    /// @copydoc owner
    [[nodiscard]] graph_type const& owner() const {
        if (auto g = optional_owner()) return *g;
        util::throw_exception(std::domain_error("the vertex is orphaned"));
    }

    /**
     * @brief returns the graph which owns this vertex.
     * @return the owner
     * @return empty if this vertex is orphaned
     */
    [[nodiscard]] virtual util::optional_ptr<graph_type> optional_owner() noexcept = 0;

    /// @copydoc optional_owner()
    [[nodiscard]] virtual util::optional_ptr<graph_type const> optional_owner() const noexcept = 0;

    /**
     * @brief returns list of input ports.
     * @return input ports
     */
    [[nodiscard]] virtual util::sequence_view<input_port_type> input_ports() noexcept = 0;

    /// @copydoc input_ports()
    [[nodiscard]] virtual util::sequence_view<input_port_type const> input_ports() const noexcept = 0;

    /**
     * @brief returns list of output ports.
     * @return output ports
     */
    [[nodiscard]] virtual util::sequence_view<output_port_type> output_ports() noexcept = 0;

    /// @copydoc output_ports()
    [[nodiscard]] virtual util::sequence_view<output_port_type const> output_ports() const noexcept = 0;

    /**
     * @brief handles when this vertex is joined into the given graph.
     * @param graph the owner graph, or nullptr to leave
     */
    virtual void on_join(graph_type* graph) noexcept = 0;

    /**
     * @brief handles when this vertex is left from the joined graph.
     */
    void on_leave() noexcept {
        on_join(nullptr);
    }

protected:
    /**
     * @brief creates a new instance.
     */
    vertex_base() = default;
};

} // namespace takatori::graph
