#pragma once

#include <iostream>

#include "step_kind.h"

#include "takatori/graph/graph.h"

#include "takatori/util/optional_ptr.h"
#include "takatori/util/reference_list_view.h"

namespace takatori::plan {

/**
 * @brief a list view of steps.
 * @tparam T the step type
 */
template<class T>
using step_list_view = util::reference_list_view<util::double_pointer_extractor<T>>;

/**
 * @brief represents a step of execution plan.
 */
class step {
public:
    /// @brief the graph type
    using graph_type = graph::graph<step>;

    virtual ~step() = default;
    step(step const& other) = delete;
    step& operator=(step const& other) = delete;
    step(step&& other) noexcept = delete;
    step& operator=(step&& other) noexcept = delete;

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
     * @brief returns the graph which owns this step.
     * @return the owner
     * @return empty if this vertex is orphaned
     */
    util::optional_ptr<graph_type> optional_owner() noexcept;

    /// @copydoc optional_owner()
    util::optional_ptr<graph_type const> optional_owner() const noexcept;

    /**
     * @brief returns the kind of this step.
     * @return the step kind
     */
    virtual step_kind kind() const noexcept = 0;

    /**
     * @brief returns a clone of this object.
     * @param creator the object creator
     * @return the created clone
     */
    virtual step* clone(util::object_creator creator) const& = 0;

    /// @copydoc clone()
    virtual step* clone(util::object_creator creator) && = 0;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(step const& a, step const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(step const& a, step const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, step const& value);

    /**
     * @brief handles when this step is joined into the given graph.
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
    step() = default;

    /**
     * @brief returns whether or not this steps is equivalent to the target one.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param other the target step
     * @return true if the both are equivalent
     * @return false otherwise
     */
    virtual bool equals(step const& other) const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;

private:
    graph_type* owner_ {};
};

} // namespace takatori::plan
