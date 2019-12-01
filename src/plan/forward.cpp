#include "takatori/plan/forward.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"
#include "takatori/util/optional_print_support.h"
#include "takatori/util/vector_print_support.h"

namespace takatori::plan {

forward::forward(util::object_creator creator) noexcept
    : exchange(creator)
    , columns_(creator.allocator<descriptor::variable>())
    , limit_(std::nullopt)
{}

forward::forward(
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> columns,
        std::optional<size_type> limit,
        util::object_creator creator) noexcept
    : exchange(creator)
    , columns_(std::move(columns))
    , limit_(std::move(limit))
{}

forward::forward(std::optional<size_type> limit, util::object_creator creator) noexcept
    : forward(
            decltype(columns_) { creator.allocator<descriptor::variable>() },
            std::move(limit),
            creator)
{}

forward::forward(
        std::initializer_list<descriptor::variable> columns,
        std::optional<size_type> limit) noexcept
    : forward(
            decltype(columns_) { columns.begin(), columns.end() },
            std::move(limit))
{}

forward::forward(forward const& other, util::object_creator creator)
    : forward(
            { other.columns_, creator.allocator<descriptor::variable>() },
            other.limit_,
            creator)
{}

forward::forward(forward&& other, util::object_creator creator)
    : forward(
            { std::move(other.columns_), creator.allocator<descriptor::variable>() },
            std::move(other.limit_),
            creator)
{}

step_kind forward::kind() const noexcept {
    return tag;
}

forward* forward::clone(util::object_creator creator) const& {
    return creator.create_object<forward>(*this, creator);
}

forward* forward::clone(util::object_creator creator) && {
    return creator.create_object<forward>(std::move(*this), creator);
}

util::sequence_view<descriptor::variable const> forward::input_columns() const noexcept {
    return util::sequence_view { columns_ };
}

util::sequence_view<descriptor::variable const> forward::output_columns() const noexcept {
    return util::sequence_view { columns_ };
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& forward::columns() noexcept {
    return columns_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& forward::columns() const noexcept {
    return columns_;
}

std::optional<forward::size_type> const& forward::limit() const noexcept {
    return limit_;
}

forward& forward::limit(std::optional<forward::size_type> limit) noexcept {
    limit_ = std::move(limit);
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
