#include <takatori/statement/execute.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

execute::execute(plan::graph_type&& execution_plan) noexcept
    : execution_plan_(std::move(execution_plan))
{}

execute::execute(execute const& other, util::object_creator creator) noexcept
    : execute(plan::graph_type { creator })
{
    merge_into(other.execution_plan_, execution_plan_, creator);
}

execute::execute(execute&& other, util::object_creator) noexcept
    : execute(std::move(other.execution_plan_))
{}

statement_kind execute::kind() const noexcept {
    return tag;
}

execute* execute::clone(util::object_creator creator) const& {
    return creator.create_object<execute>(*this, creator);
}

execute* execute::clone(util::object_creator creator) && {
    return creator.create_object<execute>(std::move(*this), creator);
}

plan::graph_type& execute::execution_plan() noexcept {
    return execution_plan_;
}

plan::graph_type const& execute::execution_plan() const noexcept {
    return execution_plan_;
}

bool operator==(execute const& a, execute const& b) noexcept {
    return std::addressof(a) == std::addressof(b);
}

bool operator!=(execute const& a, execute const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, execute const& value) {
    return out << value.kind() << "("
               << "#steps=" << value.execution_plan().size() << ")";
}

bool execute::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<execute>(other);
}

std::ostream& execute::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
