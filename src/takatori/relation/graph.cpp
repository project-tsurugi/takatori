#include <takatori/relation/graph.h>

#include <takatori/util/assertion.h>
#include <takatori/util/exception.h>
#include <takatori/util/string_builder.h>

#include <takatori/graph/topological_sort.h>

#include <takatori/relation/details/graph_merger.h>

namespace takatori::relation {

using ::takatori::graph::topological_sort;

using ::takatori::util::throw_exception;
using ::takatori::util::string_builder;

void merge_into(
        graph_type const& source,
        graph_type& destination,
        util::object_creator creator) {
    details::graph_merger merger { destination, creator };
    merger.add(source);
    merger.resolve();
}

void merge_into(
        graph_type&& source,
        graph_type& destination,
        util::object_creator creator) {
    details::graph_merger merger { destination, creator };
    merger.add(std::move(source));
    merger.resolve();
}

graph_type release(graph_type& source, util::sequence_view<expression const*> elements) {
    graph_type results { source.get_object_creator() };
    std::vector<expression*, util::object_allocator<expression*>> migrated {
            source.get_object_creator().template allocator<expression*>(),
    };
    migrated.reserve(elements.size());

    // migrate all elements in the source graph
    results.reserve(elements.size());
    for (auto const* element : elements) {
        if (auto entity = source.release(*element)) {
            auto&& r = results.insert(std::move(entity));
            BOOST_ASSERT(std::addressof(r) == element); // NOLINT
            migrated.emplace_back(std::addressof(r));
        } else {
            throw_exception(std::invalid_argument(string_builder {}
                    << "migration target must be in the source graph: "
                    << *element
                    << string_builder::to_string));
        }
    }

    // disconnect between released and left expressions
    for (auto* element : migrated) {
        for (auto&& p : element->input_ports()) {
            if (auto opposite = p.opposite();
                    opposite
                            && !results.contains(opposite->owner())) {
                p.disconnect_from(*opposite);
            }
        }
    }
    return results;
}

namespace {

template<class T>
void upstreams0(T& expr, std::function<void(T&)> const& consumer) {
    for (auto&& port : expr.input_ports()) {
        if (auto opposite = port.opposite()) {
            consumer(opposite->owner());
        }
    }
}

template<class T>
void downstreams0(T& expr, std::function<void(T&)> const& consumer) {
    for (auto&& port : expr.output_ports()) {
        if (auto opposite = port.opposite()) {
            consumer(opposite->owner());
        }
    }
}

template<class Graph, class Expr>
void enumerate_top0(Graph& g, std::function<void(Expr&)> const& consumer) {
    for (auto&& e : g) {
        if (e.input_ports().empty()) {
            consumer(e);
        }
    }
}

template<class Graph, class Expr>
void enumerate_bottom0(Graph& g, std::function<void(Expr&)> const& consumer) {
    for (auto&& e : g) {
        if (e.output_ports().empty()) {
            consumer(e);
        }
    }
}

struct upstream_enumerator {
    void operator()(expression& expr, consumer_type const& consumer) const {
        upstreams0(expr, consumer);
    }
    void operator()(expression const& expr, const_consumer_type const& consumer) const {
        upstreams0(expr, consumer);
    }
};

struct downstream_enumerator {
    void operator()(expression& expr, consumer_type const& consumer) const {
        downstreams0(expr, consumer);
    }
    void operator()(expression const& expr, const_consumer_type const& consumer) const {
        downstreams0(expr, consumer);
    }
};

} // namespace

bool has_upstream(expression const& expr) {
    return !expr.input_ports().empty();
}

bool has_downstream(expression const& expr) {
    return !expr.output_ports().empty();
}

void enumerate_top(graph_type& g, consumer_type const& consumer) {
    enumerate_top0(g, consumer);
}

void enumerate_top(graph_type const& g, const_consumer_type const& consumer) {
    enumerate_top0(g, consumer);
}

void enumerate_bottom(graph_type& g, consumer_type const& consumer) {
    enumerate_bottom0(g, consumer);
}

void enumerate_bottom(graph_type const& g, const_consumer_type const& consumer) {
    enumerate_bottom0(g, consumer);
}

void enumerate_upstream(expression& expr, consumer_type const& consumer) {
    upstreams0(expr, consumer);
}

void enumerate_upstream(expression const& expr, const_consumer_type const& consumer) {
    upstreams0(expr, consumer);
}

void enumerate_downstream(expression& expr, consumer_type const& consumer) {
    downstreams0(expr, consumer);
}

void enumerate_downstream(expression const& expr, const_consumer_type const& consumer) {
    downstreams0(expr, consumer);
}

void sort_from_upstream(graph_type& g, consumer_type const& consumer, util::object_creator creator) {
    topological_sort<upstream_enumerator>(g, consumer, creator);
}

void sort_from_upstream(graph_type const& g, const_consumer_type const& consumer, util::object_creator creator) {
    topological_sort<upstream_enumerator>(g, consumer, creator);
}

void sort_from_downstream(graph_type& g, consumer_type const& consumer, util::object_creator creator) {
    topological_sort<downstream_enumerator>(g, consumer, creator);
}

void sort_from_downstream(graph_type const& g, const_consumer_type const& consumer, util::object_creator creator) {
    topological_sort<downstream_enumerator>(g, consumer, creator);
}

} // namespace takatori::relation
