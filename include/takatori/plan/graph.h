#pragma once

#include <functional>

#include <takatori/graph/graph.h>

#include "step.h"

namespace takatori::plan {

/**
 * @brief the relational operator graph type.
 */
using graph_type = takatori::graph::graph<step>;

/// @brief the consumer type.
using consumer_type = std::function<void(step&)>;

/// @brief the consumer type for const objects.
using const_consumer_type = std::function<void(step const&)>;

/**
 * @brief enumerates upstream steps.
 */
struct upstream_enumerator {
    /**
     * @brief extracts the upstream steps of the given one.
     * @param s the target step
     * @param consumer the result consumer
     */
    void operator()(step& s, consumer_type const& consumer) const;

    /// @copydoc operator()()
    void operator()(step const& s, const_consumer_type const& consumer) const;
};

/**
 * @brief enumerates downstream steps.
 */
struct downstream_enumerator {
    /**
     * @brief extracts the downstream steps of the given one.
     * @param s the target step
     * @param consumer the result consumer
     */
    void operator()(step& s, std::function<void(step&)> const& consumer) const;

    /// @copydoc operator()()
    void operator()(step const& s, std::function<void(step const&)> const& consumer) const;
};

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

} // namespace takatori::plan
