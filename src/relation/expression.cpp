#include <takatori/relation/expression.h>

#include <takatori/util/exception.h>

namespace takatori::relation {

using ::takatori::util::throw_exception;

bool expression::is_orphan() const noexcept {
    return !optional_owner();
}

expression::graph_type& expression::owner() {
    if (auto g = optional_owner()) return *g;
    throw_exception(std::domain_error("the vertex is orphaned"));
}

expression::graph_type const& expression::owner() const {
    if (auto g = optional_owner()) return *g;
    throw_exception(std::domain_error("the vertex is orphaned"));
}

util::optional_ptr<expression::graph_type> expression::optional_owner() noexcept {
    return util::optional_ptr { owner_ };
}

util::optional_ptr<expression::graph_type const> expression::optional_owner() const noexcept {
    return util::optional_ptr { owner_ };
}

document::region& expression::region() noexcept {
    return region_;
}

document::region const& expression::region() const noexcept {
    return region_;
}

void expression::on_join(graph_type* graph) noexcept {
    owner_ = graph;
}

void expression::on_leave() noexcept {
    owner_ = nullptr;
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
