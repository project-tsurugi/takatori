#include "takatori/relation/step/take_group.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"
#include "takatori/util/vector_print_support.h"

namespace takatori::relation::step {

take_group::take_group(
        descriptor::relation source,
        std::vector<column, util::object_allocator<column>> columns,
        util::object_creator creator) noexcept
    : output_(*this, 0, creator)
    , source_(std::move(source))
    , columns_(std::move(columns))
{}

take_group::take_group(
        descriptor::relation source,
        std::initializer_list<column> columns)
    : take_group(
            std::move(source),
            { columns.begin(), columns.end() })
{}


take_group::take_group(take_group const& other, util::object_creator creator)
    : take_group(
            other.source_,
            { other.columns_, creator.allocator<column>() },
            creator)
{}

take_group::take_group(take_group&& other, util::object_creator creator)
    : take_group(
            std::move(other.source_),
            { std::move(other.columns_), creator.allocator<column>() },
            creator)
{}

expression_kind take_group::kind() const noexcept {
    return tag;
}

take_group* take_group::clone(util::object_creator creator) const& {
    return creator.create_object<take_group>(*this, creator);
}

take_group* take_group::clone(util::object_creator creator)&& {
    return creator.create_object<take_group>(std::move(*this), creator);
}


util::sequence_view<take_group::input_port_type> take_group::input_ports() noexcept {
    return {};
}

util::sequence_view<take_group::input_port_type const> take_group::input_ports() const noexcept {
    return {};
}

util::sequence_view<take_group::output_port_type> take_group::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<take_group::output_port_type const> take_group::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

take_group::output_port_type& take_group::output() noexcept {
    return output_;
}

take_group::output_port_type const& take_group::output() const noexcept {
    return output_;
}

descriptor::relation const& take_group::source() const noexcept {
    return source_;
}

take_group& take_group::source(descriptor::relation source) noexcept {
    source_ = std::move(source);
    return *this;
}

std::vector<take_group::column, util::object_allocator<take_group::column>>& take_group::columns() noexcept {
    return columns_;
}

std::vector<take_group::column, util::object_allocator<take_group::column>> const& take_group::columns() const noexcept {
    return columns_;
}

bool operator==(take_group const& a, take_group const& b) noexcept {
    return a.source() == b.source()
        && a.columns() == b.columns();
}

bool operator!=(take_group const& a, take_group const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, take_group const& value) {
    return out << value.kind() << "("
               << "source=" << value.source() << ", "
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool take_group::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<take_group>(other);
}

std::ostream& take_group::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
