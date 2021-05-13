#include <takatori/relation/project.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::relation {

project::project(std::vector<column> columns) noexcept
    : input_(*this, 0)
    , output_(*this, 0)
    , columns_(*this, std::move(columns))
{}

project::project(std::initializer_list<util::rvalue_reference_wrapper<column>> columns)
    : project({ columns.begin(), columns.end() })
{}

project::project(util::clone_tag_t, project const& other)
    : project(
            tree::forward(other.columns_))
{}

project::project(util::clone_tag_t, project&& other)
    : project(
            tree::forward(std::move(other.columns_)))
{}

expression_kind project::kind() const noexcept {
    return tag;
}

util::sequence_view<project::input_port_type> project::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<project::input_port_type const> project::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<project::output_port_type> project::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<project::output_port_type const> project::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

project* project::clone() const& {
    return new project(util::clone_tag, *this); // NOLINT
}

project* project::clone()&& {
    return new project(util::clone_tag, std::move(*this)); // NOLINT;
}

project::input_port_type& project::input() noexcept {
    return input_;
}

project::input_port_type const& project::input() const noexcept {
    return input_;
}

project::output_port_type& project::output() noexcept {
    return output_;
}

project::output_port_type const& project::output() const noexcept {
    return output_;
}

tree::tree_fragment_vector<project::column>& project::columns() noexcept {
    return columns_;
}

tree::tree_fragment_vector<project::column> const& project::columns() const noexcept {
    return columns_;
}

bool operator==(project const& a, project const& b) noexcept {
    return a.columns() == b.columns();
}

bool operator!=(project const& a, project const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, project const& value) {
    return out << value.kind() << "("
               << "columns=" << value.columns() << ")";
}

bool project::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<project>(other);
}

std::ostream& project::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
