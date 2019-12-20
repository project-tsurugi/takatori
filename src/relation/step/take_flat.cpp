#include <takatori/relation/step/take_flat.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::step {

take_flat::take_flat(
        descriptor::relation source,
        std::vector<column, util::object_allocator<column>> columns,
        util::object_creator creator) noexcept
    : output_(*this, 0, creator)
    , source_(std::move(source))
    , columns_(std::move(columns))
{}

take_flat::take_flat(
        descriptor::relation source,
        std::initializer_list<column> columns)
    : take_flat(
            std::move(source),
            { columns.begin(), columns.end() })
{}


take_flat::take_flat(take_flat const& other, util::object_creator creator)
    : take_flat(
            other.source_,
            { other.columns_, creator.allocator<column>() },
            creator)
{}

take_flat::take_flat(take_flat&& other, util::object_creator creator)
    : take_flat(
            std::move(other.source_),
            { std::move(other.columns_), creator.allocator<column>() },
            creator)
{}

expression_kind take_flat::kind() const noexcept {
    return tag;
}

take_flat* take_flat::clone(util::object_creator creator) const& {
    return creator.create_object<take_flat>(*this, creator);
}

take_flat* take_flat::clone(util::object_creator creator)&& {
    return creator.create_object<take_flat>(std::move(*this), creator);
}


util::sequence_view<take_flat::input_port_type> take_flat::input_ports() noexcept {
    return {};
}

util::sequence_view<take_flat::input_port_type const> take_flat::input_ports() const noexcept {
    return {};
}

util::sequence_view<take_flat::output_port_type> take_flat::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<take_flat::output_port_type const> take_flat::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

take_flat::output_port_type& take_flat::output() noexcept {
    return output_;
}

take_flat::output_port_type const& take_flat::output() const noexcept {
    return output_;
}

descriptor::relation const& take_flat::source() const noexcept {
    return source_;
}

take_flat& take_flat::source(descriptor::relation source) noexcept {
    source_ = std::move(source);
    return *this;
}

std::vector<take_flat::column, util::object_allocator<take_flat::column>>& take_flat::columns() noexcept {
    return columns_;
}

std::vector<take_flat::column, util::object_allocator<take_flat::column>> const& take_flat::columns() const noexcept {
    return columns_;
}

bool operator==(take_flat const& a, take_flat const& b) noexcept {
    return a.source() == b.source()
        && a.columns() == b.columns();
}

bool operator!=(take_flat const& a, take_flat const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, take_flat const& value) {
    return out << value.kind() << "("
               << "source=" << value.source() << ", "
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool take_flat::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<take_flat>(other);
}

std::ostream& take_flat::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
