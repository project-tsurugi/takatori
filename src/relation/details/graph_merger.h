#pragma once

#include <unordered_map>

#include "takatori/relation/expression.h"

#include "takatori/graph/graph.h"
#include "takatori/graph/port.h"

namespace takatori::relation::details {

class graph_merger {
public:
    using node_type = expression;
    using input_port_type = expression::input_port_type;
    using output_port_type = expression::output_port_type;
    using port_index_type = output_port_type::index_type;
    using graph_type = expression::graph_type;
    using node_mapping_entry_type = std::pair<expression const* const, expression*>;
    using node_mapping_type = std::unordered_map<
            std::remove_const_t<node_mapping_entry_type::first_type>,
            node_mapping_entry_type::second_type,
            std::hash<std::remove_const_t<node_mapping_entry_type::first_type>>,
            std::equal_to<>,
            util::object_allocator<node_mapping_entry_type>>;

    explicit graph_merger(graph_type& destination, util::object_creator creator = {}) noexcept;

    graph_merger& add(graph_type const& source);

    graph_merger& add(graph_type&& source);

    graph_type& resolve();

    graph_type& destination() const noexcept;

    node_mapping_type const& mappings() const noexcept;

    util::optional_ptr<node_type> find(node_type const& source) const noexcept;

private:
    graph_type& destination_;
    node_mapping_type mappings_;

    void repair_connections(node_type const& source, node_type& destination);
    void repair_opposite(input_port_type const& source, input_port_type& destination);
};

} // namespace takatori::relation::details
