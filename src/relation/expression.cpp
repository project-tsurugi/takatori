#include "takatori/relation/expression.h"

#include "details/graph_merger.h"

namespace takatori::relation {

bool expression::is_orphan() const noexcept {
    return !optional_owner();
}

expression::graph_type& expression::owner() {
    if (auto g = optional_owner()) return *g;
    throw std::domain_error("the vertex is orphaned");
}

expression::graph_type const& expression::owner() const {
    if (auto g = optional_owner()) return *g;
    throw std::domain_error("the vertex is orphaned");
}

util::optional_ptr<expression::graph_type> expression::optional_owner() noexcept {
    return util::optional_ptr { owner_ };
}

util::optional_ptr<expression::graph_type const> expression::optional_owner() const noexcept {
    return util::optional_ptr { owner_ };
}

void expression::on_join(graph_type* graph) noexcept {
    owner_ = graph;
}

void expression::on_leave() noexcept {
    owner_ = nullptr;
    // FIXME: dispose all connections?
}

bool operator==(expression const& a, expression const& b) noexcept {
    return a.equals(b);
}

bool operator!=(expression const& a, expression const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, expression const& value) {
    return value.print_to(out);
}

void merge_into(
        expression::graph_type const& source,
        expression::graph_type& destination,
        util::object_creator creator) {
    details::graph_merger merger { destination, creator };
    merger.add(source);
    merger.resolve();
}

void merge_into(
        expression::graph_type&& source,
        expression::graph_type& destination,
        util::object_creator creator) {
    details::graph_merger merger { destination, creator };
    merger.add(std::move(source));
    merger.resolve();
}

} // namespace takatori::relation
