#pragma once

#include <iostream>

#include "expression_kind.h"

#include "takatori/graph/vertex_base.h"

#include "takatori/tree/tree_element_base.h"

#include "takatori/util/object_creator.h"
#include "takatori/util/optional_ptr.h"

namespace takatori::relation {

/**
 * @brief a root model of relational algebra expressions.
 */
class expression : public graph::vertex_base<expression>, public tree::tree_element_base {
public:
    /// @brief the input port type.
    using input_port_type = graph::input_port<expression>;

    /// @brief the output port type.
    using output_port_type = graph::output_port<expression>;

    /**
     * @brief returns the kind of this expression.
     * @return the expression kind
     */
    virtual expression_kind kind() const noexcept = 0;

    /**
     * @brief returns list of input ports.
     * @return input ports
     */
    util::sequence_view<input_port_type> input_ports() noexcept override = 0;

    /// @copydoc input_ports()
    util::sequence_view<input_port_type const> input_ports() const noexcept override = 0;

    /**
     * @brief returns list of output ports.
     * @return output ports
     */
    util::sequence_view<output_port_type> output_ports() noexcept override = 0;

    /// @copydoc output_ports()
    util::sequence_view<output_port_type const> output_ports() const noexcept override = 0;

    /**
     * @brief returns a clone of this object.
     * @details The created clone will become orphaned vertex, that is, this never copies the following:
     *   @li the vertex ID
     *   @li the owner graph
     *   @li the input/output opposites
     * @param creator the object creator
     * @return the created clone
     */
    virtual expression* clone(util::object_creator creator) const& = 0;

    /// @copydoc clone()
    virtual expression* clone(util::object_creator creator) && = 0;

    /**
     * @brief returns the vertex ID.
     * @return the vertex ID
     * @return orphaned_id if this expression is orphaned from the relational expression graph
     */
    id_type id() const noexcept final;

    /**
     * @brief returns the graph which owns this expression.
     * @return the owner
     * @return empty if this vertex is orphaned
     */
    util::optional_ptr<graph_type> optional_owner() noexcept final;

    /// @copydoc optional_owner()
    util::optional_ptr<graph_type const> optional_owner() const noexcept final;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(expression const& a, expression const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(expression const& a, expression const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, expression const& value);

    /**
     * @brief handles when this expression is joined into the given graph.
     * @param graph the owner graph, or nullptr to leave
     * @param id the vertex ID, or orphaned_id to leave
     */
    void on_join(graph_type* graph, id_type id) noexcept final;

protected:
    /**
     * @brief creates a new instance.
     */
    expression() = default;

    /**
     * @brief returns whether or not this expressions is equivalent to the target one.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param other the target expression
     * @return true if the both are equivalent
     * @return false otherwise
     */
    virtual bool equals(expression const& other) const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;

private:
    graph_type* owner_ {};
    id_type id_ { orphaned_id };
};

} // namespace takatori::relation
