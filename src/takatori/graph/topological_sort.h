#pragma once

#include <functional>
#include <vector>
#include <unordered_set>

#include <takatori/util/infect_qualifier.h>

namespace takatori::graph {

template<class Enumerator, class Graph>
void topological_sort(
        Graph& g,
        std::function<void(util::infect_const_t<Graph, typename Graph::element_type>&)> const& consumer) {
    using enumerator_type = Enumerator;
    using element_type = util::infect_const_t<Graph, typename Graph::element_type>&;
    using reference = std::add_lvalue_reference_t<element_type>;
    using pointer = std::add_pointer_t<element_type>;

    struct dfs_entry {
        pointer const element_ptr {};
        bool visited { false };
    };
    std::vector<dfs_entry> dfs_stack {};
    dfs_stack.reserve(g.size());

    std::unordered_set<
            pointer,
            std::hash<pointer>,
            std::equal_to<>> saw {};
    saw.reserve(g.size());

    for (auto&& e : g) {
        // do nothing if already visited
        if (saw.find(std::addressof(e)) != saw.end()) {
            continue;
        }
        dfs_stack.emplace_back(dfs_entry { std::addressof(e) });
        while (!dfs_stack.empty()) {
            auto&& top = dfs_stack.back();
            if (!top.visited) {
                if (auto [it, success] = saw.emplace(top.element_ptr); success) {
                    (void) it;
                    // extract successors into top of the stack, only if the first time visit of this entry
                    enumerator_type{}(*top.element_ptr, [&](reference next) {
                        if (saw.find(std::addressof(next)) == saw.end()) {
                            dfs_stack.emplace_back(dfs_entry { std::addressof(next) });
                        }
                    });
                    // revisit current entry later
                    top.visited = true;
                } else {
                    // already visited via other paths
                    dfs_stack.pop_back();
                }
            } else {
                // already sorted at the revisit, because all successors have been already visited
                consumer(*top.element_ptr);
                dfs_stack.pop_back();
            }
        }
    }
}

} // namespace takatori::graph
