#include <takatori/relation/intermediate/limit.h>

#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::intermediate {

limit::limit(
        std::optional<size_type> count,
        std::vector<descriptor::variable> group_keys,
        std::vector<sort_key> sort_keys) noexcept
    : input_(*this, 0)
    , output_(*this, 0)
    , count_(count)
    , group_keys_(std::move(group_keys))
    , sort_keys_(std::move(sort_keys))
{}

limit::limit(
        std::optional<size_type> count,
        std::initializer_list<descriptor::variable> group_keys,
        std::initializer_list<sort_key> sort_keys)
    : limit(
            count,
            { group_keys.begin(), group_keys.end() },
            { sort_keys.begin(), sort_keys.end() })
{}

limit::limit(util::clone_tag_t, limit const& other)
    : limit(
            other.count_,
            { other.group_keys_ },
            { other.sort_keys_ })
{}

limit::limit(util::clone_tag_t, limit&& other)
    : limit(
            other.count_,
            { std::move(other.group_keys_) },
            { std::move(other.sort_keys_) })
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

limit* limit::clone() const& {
    return new limit(util::clone_tag, *this); // NOLINT
}

limit* limit::clone()&& {
    return new limit(util::clone_tag, std::move(*this)); // NOLINT;
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
    count_ = count;
    return *this;
}

std::vector<descriptor::variable>& limit::group_keys() noexcept {
    return group_keys_;
}

std::vector<descriptor::variable> const& limit::group_keys() const noexcept {
    return group_keys_;
}

std::vector<limit::sort_key>& limit::sort_keys() noexcept {
    return sort_keys_;
}

std::vector<limit::sort_key> const& limit::sort_keys() const noexcept {
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
