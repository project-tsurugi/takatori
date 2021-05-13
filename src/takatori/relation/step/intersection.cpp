#include <takatori/relation/step/intersection.h>

#include <takatori/util/downcast.h>

namespace takatori::relation::step {

intersection::intersection() noexcept
    : input_(*this, 0)
    , output_(*this, 0)
{}

intersection::intersection(util::clone_tag_t, intersection const&)  noexcept : intersection {} {}

intersection::intersection(util::clone_tag_t, intersection&&) noexcept : intersection {} {}

expression_kind intersection::kind() const noexcept {
    return tag;
}

intersection* intersection::clone() const& {
    return new intersection(util::clone_tag, *this); // NOLINT
}

intersection* intersection::clone()&& {
    return new intersection(util::clone_tag, std::move(*this)); // NOLINT;
}

util::sequence_view<intersection::input_port_type> intersection::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<intersection::input_port_type const> intersection::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<intersection::output_port_type> intersection::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<intersection::output_port_type const> intersection::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

intersection::input_port_type& intersection::input() noexcept {
    return input_;
}

intersection::input_port_type const& intersection::input() const noexcept {
    return input_;
}

intersection::output_port_type& intersection::output() noexcept {
    return output_;
}

intersection::output_port_type const& intersection::output() const noexcept {
    return output_;
}

bool operator==(intersection const&, intersection const&) noexcept {
    return true;
}

bool operator!=(intersection const& a, intersection const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, intersection const& value) {
    return out << value.kind() << "()";
}

bool intersection::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<intersection>(other);
}

std::ostream& intersection::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
