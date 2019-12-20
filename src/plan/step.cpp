#include <takatori/plan/step.h>

namespace takatori::plan {

bool step::is_orphan() const noexcept {
    return !optional_owner();
}

step::graph_type& step::owner() {
    if (auto g = optional_owner()) return *g;
    throw std::domain_error("the vertex is orphaned");
}

step::graph_type const& step::owner() const {
    if (auto g = optional_owner()) return *g;
    throw std::domain_error("the vertex is orphaned");
}

util::optional_ptr<step::graph_type> step::optional_owner() noexcept {
    return util::optional_ptr { owner_ };
}

util::optional_ptr<step::graph_type const> step::optional_owner() const noexcept {
    return util::optional_ptr { owner_ };
}

void step::on_join(graph_type* graph) noexcept {
    owner_ = graph;
}

void step::on_leave() noexcept {
    on_join(nullptr);
}

bool operator==(step const& a, step const& b) noexcept {
    return a.equals(b);
}

bool operator!=(step const& a, step const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, step const& value) {
    return value.print_to(out);
}

} // namespace takatori::plan
