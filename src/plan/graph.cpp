#include <takatori/plan/graph.h>

#include <takatori/plan/process.h>
#include <takatori/plan/exchange.h>

#include <takatori/util/downcast.h>

#include "../graph/topological_sort.h"

namespace takatori::plan {

using ::takatori::util::unsafe_downcast;

template<class T>
static void upstreams0(T& s, std::function<void(T&)> const& consumer) {
    if (s.kind() == step_kind::process) {
        for (auto&& t : unsafe_downcast<process>(s).upstreams()) {
            consumer(t);
        }
    } else {
        for (auto&& t : unsafe_downcast<exchange>(s).upstreams()) {
            consumer(t);
        }
    }
}

template<class T>
static void downstreams0(T& s, std::function<void(T&)> const& consumer) {
    if (s.kind() == step_kind::process) {
        for (auto&& t : unsafe_downcast<process>(s).downstreams()) {
            consumer(t);
        }
    } else {
        for (auto&& t : unsafe_downcast<exchange>(s).downstreams()) {
            consumer(t);
        }
    }
}

void upstream_enumerator::operator()(step& s, consumer_type const& consumer) const {
    upstreams0(s, consumer);
}

void upstream_enumerator::operator()(step const& s, const_consumer_type const& consumer) const {
    upstreams0(s, consumer);
}

void downstream_enumerator::operator()(step& s, consumer_type const& consumer) const {
    downstreams0(s, consumer);
}

void downstream_enumerator::operator()(step const& s, const_consumer_type const& consumer) const {
    downstreams0(s, consumer);
}

void sort_from_downstream(graph_type& g, consumer_type const& consumer, util::object_creator creator) {
    ::takatori::graph::topological_sort<upstream_enumerator>(g, consumer, creator);
}

void sort_from_downstream(graph_type const& g, const_consumer_type const& consumer, util::object_creator creator) {
    ::takatori::graph::topological_sort<downstream_enumerator>(g, consumer, creator);
}

} // namespace takatori::plan
