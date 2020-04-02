#include <takatori/relation/intermediate/aggregate.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::intermediate {

aggregate::aggregate(
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> group_keys,
        std::vector<column, util::object_allocator<column>> columns,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
    , group_keys_(std::move(group_keys))
    , columns_(std::move(columns))
{}

aggregate::aggregate(
        std::initializer_list<descriptor::variable> group_keys,
        std::initializer_list<column> columns)
    : aggregate(
            { group_keys.begin(), group_keys.end() },
            { columns.begin(), columns.end() })
{}

aggregate::aggregate(aggregate const& other, util::object_creator creator)
    : aggregate(
            { other.group_keys_, creator.allocator<descriptor::variable>() },
            { other.columns_, creator.allocator<column>() })
{}

aggregate::aggregate(aggregate&& other, util::object_creator creator)
    : aggregate(
            { std::move(other.group_keys_), creator.allocator<descriptor::variable>() },
            { std::move(other.columns_), creator.allocator<column>() })
{}

expression_kind aggregate::kind() const noexcept {
    return tag;
}

util::sequence_view<aggregate::input_port_type> aggregate::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<aggregate::input_port_type const> aggregate::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<aggregate::output_port_type> aggregate::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<aggregate::output_port_type const> aggregate::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

aggregate* aggregate::clone(util::object_creator creator) const& {
    return creator.create_object<aggregate>(*this, creator);
}

aggregate* aggregate::clone(util::object_creator creator)&& {
    return creator.create_object<aggregate>(std::move(*this), creator);
}

aggregate::input_port_type& aggregate::input() noexcept {
    return input_;
}

aggregate::input_port_type const& aggregate::input() const noexcept {
    return input_;
}

aggregate::output_port_type& aggregate::output() noexcept {
    return output_;
}

aggregate::output_port_type const& aggregate::output() const noexcept {
    return output_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& aggregate::group_keys() noexcept {
    return group_keys_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& aggregate::group_keys() const noexcept {
    return group_keys_;
}

std::vector<aggregate::column, util::object_allocator<aggregate::column>>& aggregate::columns() noexcept {
    return columns_;
}

std::vector<aggregate::column, util::object_allocator<aggregate::column>> const& aggregate::columns() const noexcept {
    return columns_;
}

bool operator==(aggregate const& a, aggregate const& b) noexcept {
    return a.group_keys() == b.group_keys()
        && a.columns() == b.columns();
}

bool operator!=(aggregate const& a, aggregate const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, aggregate const& value) {
    return out << value.kind() << "("
               << "group_keys=" << util::print_support { value.group_keys() } << ", "
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool aggregate::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<aggregate>(other);
}

std::ostream& aggregate::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
