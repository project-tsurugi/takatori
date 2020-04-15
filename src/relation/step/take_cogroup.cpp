#include <takatori/relation/step/take_cogroup.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::step {

take_cogroup::take_cogroup(
        std::vector<group, util::object_allocator<group>> groups,
        util::object_creator creator) noexcept
    : output_(*this, 0, creator)
    , groups_(std::move(groups))
{}

take_cogroup::take_cogroup(std::initializer_list<group> groups)
    : take_cogroup({ groups.begin(), groups.end() })
{}


take_cogroup::take_cogroup(take_cogroup const& other, util::object_creator creator)
    : take_cogroup(
            decltype(groups_) { other.groups_, creator.allocator() },
            creator)
{}

take_cogroup::take_cogroup(take_cogroup&& other, util::object_creator creator)
    : take_cogroup(
            decltype(groups_) { std::move(other.groups_), creator.allocator() },
            creator)
{}

expression_kind take_cogroup::kind() const noexcept {
    return tag;
}

take_cogroup* take_cogroup::clone(util::object_creator creator) const& {
    return creator.create_object<take_cogroup>(*this, creator);
}

take_cogroup* take_cogroup::clone(util::object_creator creator)&& {
    return creator.create_object<take_cogroup>(std::move(*this), creator);
}


util::sequence_view<take_cogroup::input_port_type> take_cogroup::input_ports() noexcept {
    return {};
}

util::sequence_view<take_cogroup::input_port_type const> take_cogroup::input_ports() const noexcept {
    return {};
}

util::sequence_view<take_cogroup::output_port_type> take_cogroup::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<take_cogroup::output_port_type const> take_cogroup::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

take_cogroup::output_port_type& take_cogroup::output() noexcept {
    return output_;
}

take_cogroup::output_port_type const& take_cogroup::output() const noexcept {
    return output_;
}

std::vector<take_cogroup::group, util::object_allocator<take_cogroup::group>>& take_cogroup::groups() noexcept {
    return groups_;
}

std::vector<take_cogroup::group, util::object_allocator<take_cogroup::group>> const& take_cogroup::groups() const noexcept {
    return groups_;
}

bool operator==(take_cogroup const& a, take_cogroup const& b) noexcept {
    return a.groups() == b.groups();
}

bool operator!=(take_cogroup const& a, take_cogroup const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, take_cogroup const& value) {
    return out << value.kind() << "("
               << "groups=" << util::print_support { value.groups() } << ")";
}

bool take_cogroup::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<take_cogroup>(other);
}

std::ostream& take_cogroup::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
