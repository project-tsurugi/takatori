#include <takatori/plan/aggregate.h>

#include <algorithm>
#include <numeric>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::plan {

aggregate::aggregate(util::object_creator creator) noexcept
    : exchange(creator)
    , source_columns_(creator.allocator<descriptor::variable>())
    , destination_columns_(creator.allocator<descriptor::variable>())
    , group_keys_(creator.allocator<descriptor::variable>())
    , aggregations_(creator.allocator<aggregation>())
{}

aggregate::aggregate(
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> source_columns,
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> destination_columns,
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> group_keys,
        std::vector<aggregation, util::object_allocator<aggregation>> aggregations,
        mode_type mode,
        util::object_creator creator) noexcept
    : exchange(creator)
    , source_columns_(std::move(source_columns))
    , destination_columns_(std::move(destination_columns))
    , group_keys_(std::move(group_keys))
    , aggregations_(std::move(aggregations))
    , mode_(mode)
{}

static void add_if_absent(
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& target,
        descriptor::variable const& value) {
    if (auto iter = std::find(target.begin(), target.end(), value); iter == target.end()) {
        target.emplace_back(value);
    }
}

aggregate::aggregate(
        std::initializer_list<descriptor::variable> group_keys,
        std::initializer_list<aggregation> aggregations,
        mode_type mode)
    : aggregate(
            {},
            {},
            { group_keys.begin(), group_keys.end() },
            { aggregations.begin(), aggregations.end() },
            mode)
{
    source_columns_.reserve(
            group_keys.size()
            + std::accumulate(
                    aggregations_.begin(),
                    aggregations_.end(),
                    std::size_t { 0 },
                    [](std::size_t count, aggregation const& b) {
                        return count + b.arguments().size();
                    }));
    destination_columns_.reserve(group_keys.size() + aggregations.size());
    for (auto& v : group_keys_) {
        add_if_absent(source_columns_, v);
        add_if_absent(destination_columns_, v);
    }
    for (auto& a : aggregations_) {
        for (auto& e : a.arguments()) {
            add_if_absent(source_columns_, e);
        }
        add_if_absent(destination_columns_, a.destination());
    }
}

aggregate::aggregate(aggregate const& other, util::object_creator creator)
    : aggregate(
            { other.source_columns_, creator.allocator<descriptor::variable>() },
            { other.destination_columns_, creator.allocator<descriptor::variable>() },
            { other.group_keys_, creator.allocator<descriptor::variable>() },
            { other.aggregations_, creator.allocator<aggregation>() },
            other.mode_,
            creator)
{}

aggregate::aggregate(aggregate&& other, util::object_creator creator)
    : aggregate(
            { std::move(other.source_columns_), creator.allocator<descriptor::variable>() },
            { std::move(other.destination_columns_), creator.allocator<descriptor::variable>() },
            { std::move(other.group_keys_), creator.allocator<descriptor::variable>() },
            { std::move(other.aggregations_), creator.allocator<aggregation>() },
            other.mode_,
            creator)
{}

step_kind aggregate::kind() const noexcept {
    return tag;
}

aggregate* aggregate::clone(util::object_creator creator) const& {
    return creator.create_object<aggregate>(*this, creator);
}

aggregate* aggregate::clone(util::object_creator creator) && {
    return creator.create_object<aggregate>(std::move(*this), creator);
}

util::sequence_view<descriptor::variable const> aggregate::input_columns() const noexcept {
    return util::sequence_view { source_columns_ };
}

util::sequence_view<descriptor::variable const> aggregate::output_columns() const noexcept {
    return util::sequence_view { destination_columns_ };
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& aggregate::source_columns() noexcept {
    return source_columns_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& aggregate::source_columns() const noexcept {
    return source_columns_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& aggregate::destination_columns() noexcept {
    return destination_columns_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& aggregate::destination_columns() const noexcept {
    return destination_columns_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& aggregate::group_keys() noexcept {
    return group_keys_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& aggregate::group_keys() const noexcept {
    return group_keys_;
}

std::vector<aggregate::aggregation, util::object_allocator<aggregate::aggregation>>& aggregate::aggregations() noexcept {
    return aggregations_;
}

std::vector<aggregate::aggregation, util::object_allocator<aggregate::aggregation>> const& aggregate::aggregations() const noexcept {
    return aggregations_;
}

aggregate::mode_type aggregate::mode() const noexcept {
    return mode_;
}

aggregate& aggregate::mode(mode_type mode) noexcept {
    mode_ = mode;
    return *this;
}

bool operator==(aggregate const& a, aggregate const& b) noexcept {
    return a.source_columns() == b.source_columns()
        && a.destination_columns() == b.destination_columns()
        && a.group_keys() == b.group_keys()
        && a.aggregations() == b.aggregations()
        && a.mode() == b.mode();
}

bool operator!=(aggregate const& a, aggregate const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, aggregate const& value) {
    return out << value.kind() << "("
               << "source_columns=" << util::print_support { value.source_columns() } << ", "
               << "destination_columns=" << util::print_support { value.destination_columns() } << ", "
               << "group_keys=" << util::print_support { value.group_keys() } << ", "
               << "aggregations=" << util::print_support { value.aggregations() } << ", "
               << "mode=" << value.mode() << ")";
}

bool aggregate::equals(step const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<aggregate>(other);
}

std::ostream& aggregate::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::plan
