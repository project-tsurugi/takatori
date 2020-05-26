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
 * @brief merges expressions in the source graph into the destination.
 * @details this may create a copy of each step in source, but reorganize the connections between steps.
 * @param source the source graph
 * @param destination the destination graph
 * @param creator the object creator (for working space)
 * @see graph_type::merge()
 */
void merge_into(
        graph_type const& source,
        graph_type& destination,
        util::object_creator creator = {});

/// @copydoc merge_into()
void merge_into(
        graph_type&& source,
        graph_type& destination,
        util::object_creator creator = {});

/**
 * @brief enumerates steps which have no upstreams.
 * @param g the target graph
 * @param consumer the destination consumer
 */
void enumerate_top(graph_type& g, consumer_type const& consumer);

/// @copydoc enumerate_top()
void enumerate_top(graph_type const& g, const_consumer_type const& consumer);

/**
 * @brief enumerates steps which have no downstreams.
 * @param g the target graph
 * @param consumer the destination consumer
 */
void enumerate_bottom(graph_type& g, consumer_type const& consumer);

/// @copydoc enumerate_bottom()
void enumerate_bottom(graph_type const& g, const_consumer_type const& consumer);

/**
 * @brief returns whether or not the given step has one or more upstream steps.
 * @param s the target step
 * @return true if this has any upstream steps
 * @return false if this has no upstream steps
 */
bool has_upstream(step const& s);

/**
 * @brief returns whether or not the given step has one or more downstream steps.
 * @param s the target step
 * @return true if this has any downstream steps
 * @return false if this has no downstream steps
 */
bool has_downstream(step const& s);

/**
 * @brief enumerates the upstream steps of the given one.
 * @param s the target step
 * @param consumer the result consumer
 */
void enumerate_upstream(step& s, consumer_type const& consumer);

/// @copydoc enumerate_upstream()
void enumerate_upstream(step const& s, const_consumer_type const& consumer);

/**
 * @brief enumerates the downstream steps of the given one.
 * @param s the target step
 * @param consumer the result consumer
 */
void enumerate_downstream(step& s, consumer_type const& consumer);

/// @copydoc enumerate_downstream()
void enumerate_downstream(step const& s, const_consumer_type const& consumer);

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

} // namespace takatori::plan
