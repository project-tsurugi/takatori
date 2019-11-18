#pragma once

#include <stdexcept>

#include <cstdint>

#include "port.h"

#include "takatori/util/optional_ptr.h"
#include "takatori/util/sequence_view.h"

namespace takatori::graph {

template<class Vertex>
class graph;

/**
 * @brief an abstract implementation of vertex of graph.
 * @tparam Vertex the actual vertex type
 */
template<class Vertex>
class vertex_base {
public:
    /// @brief the vertex type
    using vertex_type = Vertex;

    /// @brief the graph type
    using graph_type = graph<vertex_type>;

    /// @brief the input port type.
    using input_port_type = input_port<vertex_type>;

    /// @brief the output port type.
    using output_port_type = output_port<vertex_type>;

    /// @brief the vertex ID type
    using id_type = std::uint64_t;

    /// @brief vertex ID for orphaned vertices.
    static constexpr id_type orphaned_id = static_cast<id_type>(-1);

    virtual ~vertex_base() = default;
    vertex_base(vertex_base const& other) = delete;
    vertex_base& operator=(vertex_base const& other) = delete;
    vertex_base(vertex_base&& other) noexcept = delete;
    vertex_base& operator=(vertex_base&& other) noexcept = delete;

    /**
     * @brief returns the vertex ID.
     * @return the vertex ID
     */
    virtual id_type id() const noexcept = 0;

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
    graph_type& owner() {
        if (auto g = optional_owner()) return *g;
        throw std::domain_error("the vertex is orphaned");
    }

    /// @copydoc owner
    graph_type const& owner() const {
        if (auto g = optional_owner()) return *g;
        throw std::domain_error("the vertex is orphaned");
    }

    /**
     * @brief returns the graph which owns this vertex.
     * @return the owner
     * @return empty if this vertex is orphaned
     */
    virtual util::optional_ptr<graph_type> optional_owner() noexcept = 0;

    /// @copydoc optional_owner()
    virtual util::optional_ptr<graph_type const> optional_owner() const noexcept = 0;

    /**
     * @brief returns list of input ports.
     * @return input ports
     */
    virtual util::sequence_view<input_port_type> input_ports() noexcept = 0;

    /// @copydoc input_ports()
    virtual util::sequence_view<input_port_type const> input_ports() const noexcept = 0;

    /**
     * @brief returns list of output ports.
     * @return output ports
     */
    virtual util::sequence_view<output_port_type> output_ports() noexcept = 0;

    /// @copydoc output_ports()
    virtual util::sequence_view<output_port_type const> output_ports() const noexcept = 0;

    /**
     * @brief handles when this vertex is joined into the given graph.
     * @param graph the owner graph, or nullptr to leave
     * @param id the vertex ID, or orphaned_id to leave
     */
    virtual void on_join(graph_type* graph, id_type id) noexcept = 0;

    /**
     * @brief handles when this vertex is left from the joined graph.
     */
    void on_leave() noexcept {
        on_join(nullptr, orphaned_id);
    }

protected:
    /**
     * @brief creates a new instance.
     */
    vertex_base() = default;
};

} // namespace takatori::graph
