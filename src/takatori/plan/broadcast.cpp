#include <takatori/plan/broadcast.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::plan {

broadcast::broadcast(
        util::object_creator creator) noexcept
    : exchange(creator)
    , columns_(creator.allocator())
{}

broadcast::broadcast(
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> columns,
        util::object_creator creator) noexcept
    : exchange(creator)
    , columns_(std::move(columns))
{}

broadcast::broadcast(std::initializer_list<descriptor::variable> columns)
    : broadcast(
        decltype(columns_) { columns.begin(), columns.end() })
{}

broadcast::broadcast(broadcast const& other, util::object_creator creator)
    : broadcast(
            decltype(columns_) { other.columns_, creator.allocator() },
            creator)
{}

broadcast::broadcast(broadcast&& other, util::object_creator creator)
    : broadcast(
            decltype(columns_) { std::move(other.columns_), creator.allocator() },
            creator)
{}

step_kind broadcast::kind() const noexcept {
    return tag;
}

broadcast* broadcast::clone(util::object_creator creator) const& {
    return creator.create_object<broadcast>(*this, creator);
}

broadcast* broadcast::clone(util::object_creator creator) && {
    return creator.create_object<broadcast>(std::move(*this), creator);
}

util::sequence_view<descriptor::variable const> broadcast::input_columns() const noexcept {
    return util::sequence_view { columns_ };
}

util::sequence_view<descriptor::variable const> broadcast::output_columns() const noexcept {
    return util::sequence_view { columns_ };
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& broadcast::columns() noexcept {
    return columns_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& broadcast::columns() const noexcept {
    return columns_;
}

bool operator==(broadcast const& a, broadcast const& b) noexcept {
    return a.columns() == b.columns();
}

bool operator!=(broadcast const& a, broadcast const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, broadcast const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool broadcast::equals(step const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<broadcast>(other);
}

std::ostream& broadcast::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::plan
