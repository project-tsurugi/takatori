#include "takatori/relation/expression.h"

namespace takatori::relation {

expression::id_type expression::id() const noexcept {
    return id_;
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
