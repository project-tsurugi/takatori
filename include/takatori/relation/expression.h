#pragma once

#include <iostream>

#include "expression_kind.h"

#include <takatori/document/region.h>

#include <takatori/graph/graph.h>
#include <takatori/graph/port.h>

#include <takatori/tree/tree_element_base.h>

#include <takatori/util/object_creator.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/sequence_view.h>

namespace takatori::relation {

/**
 * @brief a root model of relational algebra expressions.
 */
class expression : public tree::tree_element_base {
public:
    /// @brief the graph type
    using graph_type = graph::graph<expression>;

    /// @brief the input port type.
    using input_port_type = graph::input_port<expression>;

    /// @brief the output port type.
    using output_port_type = graph::output_port<expression>;

    ~expression() override = default;
    expression(expression const& other) = delete;
    expression& operator=(expression const& other) = delete;
    expression(expression&& other) noexcept = delete;
    expression& operator=(expression&& other) noexcept = delete;

    /**
     * @brief returns whether or not this vertex is orphaned.
     * @return true if this vertex does not have the owner graph
     * @return false otherwise
     */
    bool is_orphan() const noexcept;

    /**
     * @brief returns the graph which owns this vertex.
     * @return the owner
     * @throws std::domain_error if this vertex is orphaned
     * @see is_orphaned()
     * @see optional_owner()
     */
    graph_type& owner();

    /// @copydoc owner
    graph_type const& owner() const;

    /**
     * @brief returns the graph which owns this expression.
     * @return the owner
     * @return empty if this vertex is orphaned
     */
    util::optional_ptr<graph_type> optional_owner() noexcept;

    /// @copydoc optional_owner()
    util::optional_ptr<graph_type const> optional_owner() const noexcept;

    /**
     * @brief returns the kind of this expression.
     * @return the expression kind
     */
    virtual expression_kind kind() const noexcept = 0;

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
     * @brief returns a clone of this object.
     * @details The created clone will become orphaned vertex, that is, this never copies the following:
     *   @li the owner graph
     *   @li the input/output opposites
     * @param creator the object creator
     * @return the created clone
     */
    virtual expression* clone(util::object_creator creator) const& = 0;

    /// @copydoc clone()
    virtual expression* clone(util::object_creator creator) && = 0;

    /**
     * @brief returns the document region of this element.
     * @return the document region
     */
    document::region& region() noexcept;

    /// @copydoc region()
    document::region const& region() const noexcept;

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
     */
    void on_join(graph_type* graph) noexcept;

    /**
     * @brief handles when this vertex is left from the joined graph.
     */
    void on_leave() noexcept;

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
    document::region region_ {};
};

} // namespace takatori::relation
