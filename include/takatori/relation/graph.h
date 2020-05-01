#pragma once

#include <functional>

#include <takatori/graph/graph.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/sequence_view.h>

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
 * @brief merges expressions in the source graph into the destination.
 * @details this may create a copy of each expression in source, but reorganize the connections between expressions.
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
 * @brief releases the expressions in the source graph, and creates a new graph which contains the released expressions.
 * @details The connections will be kept between released expressions,
 *      but disconnected between released and left expressions.
 * @attention the migration target elements must be available even if the source was changed
 * @param elements the elements in the source graph to migrate
 * @param source the source graph
 * @throws std::invalid_argument if some target elements are not in the source graph
 */
graph_type release(graph_type& source, util::sequence_view<expression const*> elements);

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
