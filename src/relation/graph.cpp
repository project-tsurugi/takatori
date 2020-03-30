#include <takatori/relation/graph.h>

#include "../graph/topological_sort.h"

namespace takatori::relation {

template<class T>
static void upstreams0(T& expr, std::function<void(T&)> const& consumer) {
    for (auto&& port : expr.output_ports()) {
        if (auto opposite = port.opposite()) {
            consumer(opposite->owner());
        }
    }
}

template<class T>
static void downstreams0(T& expr, std::function<void(T&)> const& consumer) {
    for (auto&& port : expr.input_ports()) {
        if (auto opposite = port.opposite()) {
            consumer(opposite->owner());
        }
    }
}

void relation::upstream_enumerator::operator()(expression& expr, consumer_type const& consumer) const {
    upstreams0(expr, consumer);
}

void upstream_enumerator::operator()(expression const& expr, const_consumer_type const& consumer) const {
    upstreams0(expr, consumer);
}

void downstream_enumerator::operator()(expression& expr, consumer_type const& consumer) const {
    downstreams0(expr, consumer);
}

void downstream_enumerator::operator()(expression const& expr, const_consumer_type const& consumer) const {
    downstreams0(expr, consumer);
}

void sort_from_upstream(graph_type& g, consumer_type const& consumer, util::object_creator creator) {
    ::takatori::graph::topological_sort<upstream_enumerator>(g, consumer, creator);
}

void sort_from_downstream(graph_type& g, consumer_type const& consumer, util::object_creator creator) {
    ::takatori::graph::topological_sort<downstream_enumerator>(g, consumer, creator);
}

} // namespace takatori::relation
