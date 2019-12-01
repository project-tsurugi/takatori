#include "takatori/relation/expression.h"

namespace takatori::relation {

expression::id_type expression::id() const noexcept {
    return id_;
}

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

void expression::on_join(graph_type* graph, id_type id) noexcept {
    owner_ = graph;
    id_ = id;
}

void expression::on_leave() noexcept {
    on_join(nullptr, orphaned_id);
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

} // namespace takatori::relation
