#include <takatori/plan/broadcast.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::plan {

broadcast::broadcast(std::vector<descriptor::variable> columns) noexcept :
    columns_(std::move(columns))
{}

broadcast::broadcast(std::initializer_list<descriptor::variable> columns)
    : broadcast(
        decltype(columns_) { columns.begin(), columns.end() })
{}

broadcast::broadcast(util::clone_tag_t, broadcast const& other)
    : broadcast(
            decltype(columns_) { other.columns_ })
{}

broadcast::broadcast(util::clone_tag_t, broadcast&& other)
    : broadcast(
            decltype(columns_) { std::move(other.columns_) })
{}

step_kind broadcast::kind() const noexcept {
    return tag;
}

broadcast* broadcast::clone() const& {
    return new broadcast(util::clone_tag, *this); // NOLINT
}

broadcast* broadcast::clone() && {
    return new broadcast(util::clone_tag, std::move(*this)); // NOLINT;
}

util::sequence_view<descriptor::variable const> broadcast::input_columns() const noexcept {
    return util::sequence_view { columns_ };
}

util::sequence_view<descriptor::variable const> broadcast::output_columns() const noexcept {
    return util::sequence_view { columns_ };
}

std::vector<descriptor::variable>& broadcast::columns() noexcept {
    return columns_;
}

std::vector<descriptor::variable> const& broadcast::columns() const noexcept {
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
