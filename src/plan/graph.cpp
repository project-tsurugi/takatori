#include <takatori/plan/graph.h>

#include <unordered_map>

#include <cassert>

#include <takatori/plan/process.h>
#include <takatori/plan/exchange.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

#include "../graph/topological_sort.h"

namespace takatori::plan {

using ::takatori::util::clone_unique;
using ::takatori::util::unsafe_downcast;

template<class T, class Mapping>
void repair_upstreams(Mapping const& mapping, step const& from, step& to) {
    assert(from.kind() == to.kind()); // NOLINT
    auto&& from_t = unsafe_downcast<T>(from);
    auto&& to_t = unsafe_downcast<T>(to);
    for (auto&& from_up : from_t.upstreams()) {
        if (auto it = mapping.find(std::addressof(from_up)); it != mapping.end()) {
            auto&& to_up = unsafe_downcast<typename T::adjacent_type>(*it->second);
            to_t.add_upstream(to_up);
        }
    }
}

void merge_into(graph_type const& source, graph_type& destination, util::object_creator creator) {
    std::unordered_map<
            plan::step const*,
            plan::step*,
            std::hash<plan::step const*>,
            std::equal_to<>,
            util::object_allocator<std::pair<plan::step const* const, plan::step*>>> mapping { creator.allocator() };
    destination.reserve(destination.size() + source.size());
    mapping.reserve(source.size());
    for (auto&& e : source) {
        auto&& copy = destination.insert(clone_unique(e, creator));
        mapping.emplace(std::addressof(e), std::addressof(copy));
    }
    for (auto [source, copy] : mapping) {
        if (source->kind() == step_kind::process) {
            repair_upstreams<process>(mapping, *source, *copy);
        } else {
            repair_upstreams<exchange>(mapping, *source, *copy);
        }
    }
}

void merge_into(graph_type&& source, graph_type& destination, util::object_creator creator) {
    destination.reserve(destination.size() + source.size());
    if (source.get_object_creator() == destination.get_object_creator()) {
        destination.merge(std::move(source));
    } else {
        merge_into(std::as_const(source), destination, creator);
        source.clear();
    }
}

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

template<class Enumerator, class Graph, class Step>
static void enumerate_edge0(Graph& g, std::function<void(Step&)> const& consumer) {
    for (auto&& s : g) {
        bool found = false;
        Enumerator {}(s, [&](Step&) { found = true; });
        if (!found) {
            consumer(s);
        }
    }
}

void enumerate_top(graph_type& g, consumer_type const& consumer) {
    enumerate_edge0<upstream_enumerator>(g, consumer);
}

void enumerate_top(graph_type const& g, const_consumer_type const& consumer) {
    enumerate_edge0<upstream_enumerator>(g, consumer);
}

void enumerate_bottom(graph_type& g, consumer_type const& consumer) {
    enumerate_edge0<downstream_enumerator>(g, consumer);
}

void enumerate_bottom(graph_type const& g, const_consumer_type const& consumer) {
    enumerate_edge0<downstream_enumerator>(g, consumer);
}

void sort_from_upstream(graph_type& g, consumer_type const& consumer, util::object_creator creator) {
    ::takatori::graph::topological_sort<upstream_enumerator>(g, consumer, creator);
}

void sort_from_upstream(graph_type const& g, const_consumer_type const& consumer, util::object_creator creator) {
    ::takatori::graph::topological_sort<upstream_enumerator>(g, consumer, creator);
}

void sort_from_downstream(graph_type& g, consumer_type const& consumer, util::object_creator creator) {
    ::takatori::graph::topological_sort<downstream_enumerator>(g, consumer, creator);
}

void sort_from_downstream(graph_type const& g, const_consumer_type const& consumer, util::object_creator creator) {
    ::takatori::graph::topological_sort<downstream_enumerator>(g, consumer, creator);
}

} // namespace takatori::plan
