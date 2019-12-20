#include <takatori/relation/intermediate/limit.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::intermediate {

limit::limit(
        size_type count,
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> keys,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
    , count_(count)
    , keys_(std::move(keys))
{}

limit::limit(
        size_type count,
        std::initializer_list<descriptor::variable> keys)
    : limit(count, { keys.begin(), keys.end() })
{}

limit::limit(limit const& other, util::object_creator creator)
    : limit(
            other.count_,
            { other.keys_, creator.allocator<descriptor::variable>() },
            creator)
{}

limit::limit(limit&& other, util::object_creator creator)
    : limit(
            other.count_,
            { std::move(other.keys_), creator.allocator<descriptor::variable>() },
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

limit::size_type limit::count() const noexcept {
    return count_;
}

limit& limit::count(limit::size_type count) noexcept {
    count_ = count;
    return *this;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& limit::keys() noexcept {
    return keys_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& limit::keys() const noexcept {
    return keys_;
}

bool operator==(limit const& a, limit const& b) noexcept {
    return a.count() == b.count()
        && a.keys() == b.keys();
}

bool operator!=(limit const& a, limit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, limit const& value) {
    return out << value.kind() << "("
               << "count=" << value.count() << ", "
               << "keys=" << util::print_support { value.keys() } << ")";
}

bool limit::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<limit>(other);
}

std::ostream& limit::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
