#include <takatori/relation/intermediate/limit.h>

#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::intermediate {

limit::limit(
        std::optional<size_type> count,
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> group_keys,
        std::vector<sort_key, util::object_allocator<sort_key>> sort_keys,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
    , count_(std::move(count))
    , group_keys_(std::move(group_keys))
    , sort_keys_(std::move(sort_keys))
{}

limit::limit(
        std::optional<size_type> count,
        std::initializer_list<descriptor::variable> group_keys,
        std::initializer_list<sort_key> sort_keys)
    : limit(
            std::move(count),
            { group_keys.begin(), group_keys.end() },
            { sort_keys.begin(), sort_keys.end() })
{}

limit::limit(limit const& other, util::object_creator creator)
    : limit(
            other.count_,
            { other.group_keys_, creator.allocator<descriptor::variable>() },
            { other.sort_keys_, creator.allocator<sort_key>() },
            creator)
{}

limit::limit(limit&& other, util::object_creator creator)
    : limit(
            std::move(other.count_),
            { std::move(other.group_keys_), creator.allocator<descriptor::variable>() },
            { std::move(other.sort_keys_), creator.allocator<sort_key>() },
            creator)
{}

expression_kind limit::kind() const noexcept {
    return tag;
}

util::sequence_view<limit::input_port_type> limit::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<limit::input_port_type const> limit::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<limit::output_port_type> limit::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<limit::output_port_type const> limit::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

limit* limit::clone(util::object_creator creator) const& {
    return creator.create_object<limit>(*this, creator);
}

limit* limit::clone(util::object_creator creator)&& {
    return creator.create_object<limit>(std::move(*this), creator);
}

limit::input_port_type& limit::input() noexcept {
    return input_;
}

limit::input_port_type const& limit::input() const noexcept {
    return input_;
}

limit::output_port_type& limit::output() noexcept {
    return output_;
}

limit::output_port_type const& limit::output() const noexcept {
    return output_;
}

std::optional<limit::size_type> const& limit::count() const noexcept {
    return count_;
}

limit& limit::count(std::optional<size_type> count) noexcept {
    count_ = std::move(count);
    return *this;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& limit::group_keys() noexcept {
    return group_keys_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& limit::group_keys() const noexcept {
    return group_keys_;
}

std::vector<limit::sort_key, util::object_allocator<limit::sort_key>>& limit::sort_keys() noexcept {
    return sort_keys_;
}

std::vector<limit::sort_key, util::object_allocator<limit::sort_key>> const& limit::sort_keys() const noexcept {
    return sort_keys_;
}

bool operator==(limit const& a, limit const& b) noexcept {
    return a.count() == b.count()
        && a.group_keys() == b.group_keys()
        && a.sort_keys() == b.sort_keys();
}

bool operator!=(limit const& a, limit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, limit const& value) {
    return out << value.kind() << "("
               << "count=" << util::print_support { value.count() } << ", "
               << "group_keys=" << util::print_support { value.group_keys() } << ", "
               << "sort_keys=" << util::print_support { value.sort_keys() } << ")";
}

bool limit::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<limit>(other);
}

std::ostream& limit::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
