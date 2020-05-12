#include "graph_merger.h"

#include <takatori/util/assertion.h>

namespace takatori::relation::details {

graph_merger::graph_merger(graph_merger::graph_type& destination, util::object_creator creator) noexcept
    : destination_(destination)
    , mappings_(creator.allocator())
{}

graph_merger& graph_merger::add(graph_merger::graph_type const& source) {
    mappings_.reserve(source.size());
    destination_.reserve(source.size() + destination_.size());
    for (auto&& s : source) {
        auto&& d = destination_.insert(s);
        mappings_.emplace(std::addressof(s), std::addressof(d));
    }
    return *this;
}

graph_merger& graph_merger::add(graph_merger::graph_type&& source) {
    if (source.get_object_creator() == destination_.get_object_creator()) {
        // move
        destination_.merge(std::move(source));
    } else {
        // copy using internal merger
        graph_merger {
                destination_,
                util::object_creator { mappings_.get_allocator() },
        }.add(source).resolve();
        // explicitly remove all elements in source
        source.clear();
    }
    return *this;
}

graph_merger::graph_type& graph_merger::resolve() {
    for (auto [src, dst] : mappings_) {
        repair_connections(*src, *dst);
    }
    return destination();
}

graph_merger::graph_type& graph_merger::destination() const noexcept {
    return destination_;
}

graph_merger::node_mapping_type const& graph_merger::mappings() const noexcept {
    return mappings_;
}

void graph_merger::repair_connections(node_type const& source, node_type& destination) {
    auto&& source_inputs = source.input_ports();
    auto&& destination_inputs = destination.input_ports();
    auto source_iter = source_inputs.begin();
    auto destination_iter = destination_inputs.begin();
    while (source_iter != source_inputs.end()) {
        BOOST_ASSERT(destination_iter != destination_inputs.end()); // NOLINT
        repair_opposite(*source_iter, *destination_iter);
        ++source_iter;
        ++destination_iter;
    }
    BOOST_ASSERT(destination_iter == destination_inputs.end()); // NOLINT
}

void graph_merger::repair_opposite(input_port_type const& source, input_port_type& destination) {
    destination.disconnect_all();
    if (auto source_opposite = source.opposite()) {
        node_type const& source_upstream = source_opposite->owner();
        port_index_type output_index = source_opposite->index();
        util::optional_ptr<node_type> destination_upstream = find(source_upstream);
        BOOST_ASSERT(destination_upstream); // NOLINT
        BOOST_ASSERT(output_index < destination_upstream->output_ports().size()); // NOLINT
        auto& destination_opposite = destination_upstream->output_ports()[output_index];
        destination.connect_to(destination_opposite);
    }
}

util::optional_ptr<graph_merger::node_type> graph_merger::find(graph_merger::node_type const& source) const noexcept {
    if (auto it = mappings_.find(std::addressof(source)); it != mappings_.end()) {
        return *it->second;
    }
    return {};
}

} // namespace takatori::relation::details
