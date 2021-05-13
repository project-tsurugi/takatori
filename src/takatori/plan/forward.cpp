#include <takatori/plan/forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::plan {

forward::forward(
        std::vector<descriptor::variable> columns,
        std::optional<size_type> limit) noexcept
    : columns_(std::move(columns))
    , limit_(limit)
{}

forward::forward(std::optional<size_type> limit) noexcept
    : forward(
            decltype(columns_) {},
            limit)
{}

forward::forward(
        std::initializer_list<descriptor::variable> columns,
        std::optional<size_type> limit) noexcept
    : forward(
            decltype(columns_) { columns.begin(), columns.end() },
            limit)
{}

forward::forward(util::clone_tag_t, forward const& other)
    : forward(
            { other.columns_ },
            other.limit_)
{}

forward::forward(util::clone_tag_t, forward&& other)
    : forward(
            { std::move(other.columns_) },
            other.limit_)
{}

step_kind forward::kind() const noexcept {
    return tag;
}

forward* forward::clone() const& {
    return new forward(util::clone_tag, *this); // NOLINT
}

forward* forward::clone() && {
    return new forward(util::clone_tag, std::move(*this)); // NOLINT;
}

util::sequence_view<descriptor::variable const> forward::input_columns() const noexcept {
    return util::sequence_view { columns_ };
}

util::sequence_view<descriptor::variable const> forward::output_columns() const noexcept {
    return util::sequence_view { columns_ };
}

std::vector<descriptor::variable>& forward::columns() noexcept {
    return columns_;
}

std::vector<descriptor::variable> const& forward::columns() const noexcept {
    return columns_;
}

std::optional<forward::size_type> const& forward::limit() const noexcept {
    return limit_;
}

forward& forward::limit(std::optional<forward::size_type> limit) noexcept {
    limit_ = limit;
    return *this;
}

bool operator==(forward const& a, forward const& b) noexcept {
    return a.columns() == b.columns()
        && a.limit() == b.limit();
}

bool operator!=(forward const& a, forward const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, forward const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ", "
               << "limit=" << util::print_support { value.limit() } << ")";
}

bool forward::equals(step const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<forward>(other);
}

std::ostream& forward::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::plan
