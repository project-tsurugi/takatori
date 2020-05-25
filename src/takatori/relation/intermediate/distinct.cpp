#include <takatori/relation/intermediate/distinct.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::intermediate {

distinct::distinct(
        std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> group_keys,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
    , group_keys_(std::move(group_keys))
{}

distinct::distinct(std::initializer_list<descriptor::variable> group_keys)
    : distinct({ group_keys.begin(), group_keys.end() })
{}

distinct::distinct(distinct const& other, util::object_creator creator)
    : distinct(
            decltype(group_keys_) { other.group_keys_, creator.allocator() },
            creator)
{}

distinct::distinct(distinct&& other, util::object_creator creator)
    : distinct(
            decltype(group_keys_) { std::move(other.group_keys_), creator.allocator() },
            creator)
{}

expression_kind distinct::kind() const noexcept {
    return tag;
}

util::sequence_view<distinct::input_port_type> distinct::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<distinct::input_port_type const> distinct::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<distinct::output_port_type> distinct::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<distinct::output_port_type const> distinct::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

distinct* distinct::clone(util::object_creator creator) const& {
    return creator.create_object<distinct>(*this, creator);
}

distinct* distinct::clone(util::object_creator creator)&& {
    return creator.create_object<distinct>(std::move(*this), creator);
}

distinct::input_port_type& distinct::input() noexcept {
    return input_;
}

distinct::input_port_type const& distinct::input() const noexcept {
    return input_;
}

distinct::output_port_type& distinct::output() noexcept {
    return output_;
}

distinct::output_port_type const& distinct::output() const noexcept {
    return output_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& distinct::group_keys() noexcept {
    return group_keys_;
}

std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& distinct::group_keys() const noexcept {
    return group_keys_;
}

bool operator==(distinct const& a, distinct const& b) noexcept {
    return a.group_keys() == b.group_keys();
}

bool operator!=(distinct const& a, distinct const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, distinct const& value) {
    return out << value.kind() << "("
               << "group_keys=" << util::print_support { value.group_keys() } << ")";
}

bool distinct::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<distinct>(other);
}

std::ostream& distinct::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
