#include <takatori/relation/step/take_cogroup.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::step {

take_cogroup::take_cogroup(
        std::vector<group> groups) noexcept
    : output_(*this, 0)
    , groups_(std::move(groups))
{}

take_cogroup::take_cogroup(std::initializer_list<group> groups)
    : take_cogroup({ groups.begin(), groups.end() })
{}


take_cogroup::take_cogroup(util::clone_tag_t, take_cogroup const& other)
    : take_cogroup(
            decltype(groups_) { other.groups_ })
{}

take_cogroup::take_cogroup(util::clone_tag_t, take_cogroup&& other)
    : take_cogroup(
            decltype(groups_) { std::move(other.groups_) })
{}

expression_kind take_cogroup::kind() const noexcept {
    return tag;
}

take_cogroup* take_cogroup::clone() const& {
    return new take_cogroup(util::clone_tag, *this); // NOLINT
}

take_cogroup* take_cogroup::clone()&& {
    return new take_cogroup(util::clone_tag, std::move(*this)); // NOLINT;
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

std::vector<take_cogroup::group>& take_cogroup::groups() noexcept {
    return groups_;
}

std::vector<take_cogroup::group> const& take_cogroup::groups() const noexcept {
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
