#include <takatori/plan/graph.h>

#include <unordered_map>

#include <takatori/plan/process.h>
#include <takatori/plan/exchange.h>

#include <takatori/util/assertion.h>
#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

#include <takatori/graph/topological_sort.h>

namespace takatori::plan {

using ::takatori::graph::topological_sort;

using ::takatori::util::clone_unique;
using ::takatori::util::unsafe_downcast;

template<class T, class Mapping>
static void repair_upstreams(Mapping const& mapping, step const& from, step& to) {
    BOOST_ASSERT(from.kind() == to.kind()); // NOLINT
    auto&& from_t = unsafe_downcast<T>(from);
    auto&& to_t = unsafe_downcast<T>(to);
    for (auto&& from_up : from_t.upstreams()) {
        if (auto it = mapping.find(std::addressof(from_up)); it != mapping.end()) {
            auto&& to_up = unsafe_downcast<typename T::adjacent_type>(*it->second);
            to_t.add_upstream(to_up);
        }
    }
}

void merge_into(graph_type const& source, graph_type& destination) {
    std::unordered_map<
            plan::step const*,
            plan::step*,
            std::hash<plan::step const*>,
            std::equal_to<>> mapping {};
    destination.reserve(destination.size() + source.size());
    mapping.reserve(source.size());
    for (auto&& e : source) {
        auto&& copy = destination.insert(clone_unique(e));
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

void merge_into(graph_type&& source, graph_type& destination) {
    destination.reserve(destination.size() + source.size());
    destination.merge(std::move(source));
}

namespace {

template<class T>
void upstreams0(T& s, std::function<void(T&)> const& consumer) {
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
void downstreams0(T& s, std::function<void(T&)> const& consumer) {
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

struct upstream_enumerator {
    bool operator()(step const& s) const {
        if (s.kind() == step_kind::process) {
            return !unsafe_downcast<process>(s).upstreams().empty();
        }
        return !unsafe_downcast<exchange>(s).upstreams().empty();
    }
    void operator()(step& s, consumer_type const& consumer) const {
        upstreams0(s, consumer);
    }
    void operator()(step const& s, const_consumer_type const& consumer) const {
        upstreams0(s, consumer);
    }
};

struct downstream_enumerator {
    bool operator()(step const& s) const {
        if (s.kind() == step_kind::process) {
            return !unsafe_downcast<process>(s).downstreams().empty();
        }
        return !unsafe_downcast<exchange>(s).downstreams().empty();
    }
    void operator()(step& s, consumer_type const& consumer) const {
        downstreams0(s, consumer);
    }
    void operator()(step const& s, const_consumer_type const& consumer) const {
        downstreams0(s, consumer);
    }
};

template<class Enumerator, class Graph, class Step>
void enumerate_edge0(Graph& g, std::function<void(Step&)> const& consumer) {
    for (auto&& s : g) {
        if (!Enumerator {}(s)) {
            consumer(s);
        }
    }
}

} // namespace

bool has_upstream(step const& s) {
    return upstream_enumerator {}(s);
}

bool has_downstream(step const& s) {
    return downstream_enumerator {}(s);
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

void enumerate_upstream(step& s, consumer_type const& consumer) {
    upstreams0(s, consumer);
}

void enumerate_upstream(step const& s, const_consumer_type const& consumer) {
    upstreams0(s, consumer);
}

void enumerate_downstream(step& s, consumer_type const& consumer) {
    downstreams0(s, consumer);
}

void enumerate_downstream(step const& s, const_consumer_type const& consumer) {
    downstreams0(s, consumer);
}

void sort_from_upstream(graph_type& g, consumer_type const& consumer) {
    topological_sort<upstream_enumerator>(g, consumer);
}

void sort_from_upstream(graph_type const& g, const_consumer_type const& consumer) {
    topological_sort<upstream_enumerator>(g, consumer);
}

void sort_from_downstream(graph_type& g, consumer_type const& consumer) {
    topological_sort<downstream_enumerator>(g, consumer);
}

void sort_from_downstream(graph_type const& g, const_consumer_type const& consumer) {
    topological_sort<downstream_enumerator>(g, consumer);
}

} // namespace takatori::plan
