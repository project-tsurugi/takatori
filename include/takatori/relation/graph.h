#pragma once

#include <functional>

#include <takatori/graph/graph.h>
#include <takatori/util/object_creator.h>

#include "expression.h"

namespace takatori::relation {

/**
 * @brief the relational operator graph type.
 */
using graph_type = takatori::graph::graph<expression>;

/// @brief the consumer type.
using consumer_type = std::function<void(expression&)>;

/// @brief the consumer type for const objects.
using const_consumer_type = std::function<void(expression const&)>;

/**
 * @brief enumerates upstream expressions.
 */
struct upstream_enumerator {
    /**
     * @brief extracts the upstream expressions of the given one.
     * @param expr the target expression
     * @param consumer the result consumer
     */
    void operator()(expression& expr, consumer_type const& consumer) const;

    /// @copydoc operator()()
    void operator()(expression const& expr, const_consumer_type const& consumer) const;
};

/**
 * @brief enumerates downstream expressions.
 */
struct downstream_enumerator {
    /**
     * @brief extracts the downstream expressions of the given one.
     * @param expr the target expression
     * @param consumer the result consumer
     */
    void operator()(expression& expr, consumer_type const& consumer) const;

    /// @copydoc operator()()
    void operator()(expression const& expr, const_consumer_type const& consumer) const;
};

/**
 * @brief apply topological sort (from upstream to downstream) to the graph.
 * @param g the target graph
 * @param consumer the destination consumer
 * @param creator the object creator for temporary working area
 * @attention if the given graph is cyclic, the result may not be sorted correctly
 */
void sort_from_upstream(graph_type& g, consumer_type const& consumer, util::object_creator creator = {});

/// @copydoc sort_from_upstream()
void sort_from_upstream(graph_type const& g, const_consumer_type const& consumer, util::object_creator creator = {});

/**
 * @brief apply topological sort (from downstream to upstream) to the graph.
 * @param g the target graph
 * @param consumer the destination consumer
 * @param creator the object creator for temporary working area
 * @attention if the given graph is cyclic, the result may not be sorted correctly
 */
void sort_from_downstream(graph_type& g, consumer_type const& consumer, util::object_creator creator = {});

/// @copydoc sort_from_downstream()
void sort_from_downstream(graph_type const& g, const_consumer_type const& consumer, util::object_creator creator = {});

} // namespace takatori::relation
