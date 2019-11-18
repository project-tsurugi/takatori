#include "takatori/relation/select.h"

#include "takatori/tree/tree_element_util.h"
#include "tree/tree_fragment_vector_forward.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"

namespace takatori::relation {

select::select(std::vector<column, util::pmr::polymorphic_allocator<column>> columns) noexcept
    : columns_(*this, std::move(columns))
{}

select::select(select::column&& column)
    : select({ util::rvalue_reference_wrapper { std::move(column) } })
{}

select::select(std::initializer_list<util::rvalue_reference_wrapper<column>> columns)
    : select({ columns.begin(), columns.end() })
{}

select::select(select const& other, util::object_creator creator)
    : select(tree::forward(creator, other.columns_))
{}

select::select(select&& other, util::object_creator creator)
    : select(tree::forward(creator, std::move(other.columns_)))
{}

expression_kind select::kind() const noexcept {
    return tag;
}

util::sequence_view<select::input_port_type> select::input_ports() noexcept {
    return util::sequence_view { &input_ };
}

util::sequence_view<select::input_port_type const> select::input_ports() const noexcept {
    return util::sequence_view { &input_ };
}

util::sequence_view<select::output_port_type> select::output_ports() noexcept {
    return util::sequence_view { &output_ };
}

util::sequence_view<select::output_port_type const> select::output_ports() const noexcept {
    return util::sequence_view { &output_ };
}

select* select::clone(util::object_creator creator) const& {
    return creator.create_object<select>(*this, creator);
}

select* select::clone(util::object_creator creator)&& {
    return creator.create_object<select>(std::move(*this), creator);
}

select::input_port_type& select::input() noexcept {
    return input_;
}

select::input_port_type const& select::input() const noexcept {
    return input_;
}

select::output_port_type& select::output() noexcept {
    return output_;
}

select::output_port_type const& select::output() const noexcept {
    return output_;
}

tree::tree_fragment_vector<select::column>& select::columns() noexcept {
    return columns_;
}

tree::tree_fragment_vector<select::column> const& select::columns() const noexcept {
    return columns_;
}

bool operator==(select const& a, select const& b) noexcept {
    return a.columns_ == b.columns_;
}

bool operator!=(select const& a, select const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, select const& value) {
    return out << value.kind() << "("
               << "columns=" << value.columns() << ")";
}

bool select::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<select>(other);
}

std::ostream& select::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
