#include <takatori/plan/group.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::plan {

group::group(
        std::vector<descriptor::variable> columns,
        std::vector<descriptor::variable> group_keys,
        std::vector<sort_key> sort_keys,
        std::optional<size_type> limit,
        mode_type mode) noexcept
    : columns_(std::move(columns))
    , group_keys_(std::move(group_keys))
    , sort_keys_(std::move(sort_keys))
    , limit_(limit)
    , mode_(mode)
{}

group::group(
        std::initializer_list<descriptor::variable> columns,
        std::initializer_list<descriptor::variable> group_keys,
        std::initializer_list<sort_key> sort_keys,
        std::optional<size_type> limit,
        mode_type mode)
    : group(
            { columns.begin(), columns.end() },
            { group_keys.begin(), group_keys.end() },
            { sort_keys.begin(), sort_keys.end() },
            limit,
            mode)
{}

group::group(util::clone_tag_t, group const& other)
    : group(
            { other.columns_ },
            { other.group_keys_ },
            { other.sort_keys_ },
            other.limit_,
            other.mode_)
{}

group::group(util::clone_tag_t, group&& other)
    : group(
            { std::move(other.columns_) },
            { std::move(other.group_keys_) },
            { std::move(other.sort_keys_) },
            other.limit_,
            other.mode_)
{}

step_kind group::kind() const noexcept {
    return tag;
}

group* group::clone() const& {
    return new group(util::clone_tag, *this); // NOLINT
}

group* group::clone() && {
    return new group(util::clone_tag, std::move(*this)); // NOLINT;
}

util::sequence_view<descriptor::variable const> group::input_columns() const noexcept {
    return util::sequence_view { columns_ };
}

util::sequence_view<descriptor::variable const> group::output_columns() const noexcept {
    return util::sequence_view { columns_ };
}

std::vector<descriptor::variable>& group::columns() noexcept {
    return columns_;
}

std::vector<descriptor::variable> const& group::columns() const noexcept {
    return columns_;
}

std::vector<descriptor::variable>& group::group_keys() noexcept {
    return group_keys_;
}

std::vector<descriptor::variable> const& group::group_keys() const noexcept {
    return group_keys_;
}

std::vector<group::sort_key>& group::sort_keys() noexcept {
    return sort_keys_;
}

std::vector<group::sort_key> const& group::sort_keys() const noexcept {
    return sort_keys_;
}

std::optional<group::size_type> const& group::limit() const noexcept {
    return limit_;
}

group& group::limit(std::optional<group::size_type> limit) noexcept {
    limit_ = limit;
    return *this;
}

group::mode_type group::mode() const noexcept {
    return mode_;
}

group& group::mode(mode_type mode) noexcept {
    mode_ = mode;
    return *this;
}

bool operator==(group const& a, group const& b) noexcept {
    return a.columns() == b.columns()
        && a.group_keys() == b.group_keys()
        && a.sort_keys() == b.sort_keys()
        && a.limit() == b.limit()
        && a.mode() == b.mode();
}

bool operator!=(group const& a, group const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, group const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ", "
               << "group_keys=" << util::print_support { value.group_keys() } << ", "
               << "sort_keys=" << util::print_support { value.sort_keys() } << ", "
               << "limit=" << util::print_support { value.limit() } << ", "
               << "mode=" << value.mode() << ")";
}

bool group::equals(step const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<group>(other);
}

std::ostream& group::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::plan
