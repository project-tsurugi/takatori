#include <takatori/relation/step/flatten.h>

#include <takatori/util/downcast.h>

namespace takatori::relation::step {

flatten::flatten() noexcept
    : input_(*this, 0)
    , output_(*this, 0)
{}

flatten::flatten(util::clone_tag_t, flatten const&) : flatten{} {}

flatten::flatten(util::clone_tag_t, flatten&&) : flatten {} {}

expression_kind flatten::kind() const noexcept {
    return tag;
}

flatten* flatten::clone() const& {
    return new flatten(util::clone_tag, *this); // NOLINT
}

flatten* flatten::clone()&& {
    return new flatten(util::clone_tag, std::move(*this)); // NOLINT;
}

util::sequence_view<flatten::input_port_type> flatten::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<flatten::input_port_type const> flatten::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<flatten::output_port_type> flatten::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<flatten::output_port_type const> flatten::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

flatten::input_port_type& flatten::input() noexcept {
    return input_;
}

flatten::input_port_type const& flatten::input() const noexcept {
    return input_;
}

flatten::output_port_type& flatten::output() noexcept {
    return output_;
}

flatten::output_port_type const& flatten::output() const noexcept {
    return output_;
}

bool operator==(flatten const&, flatten const&) noexcept {
    return true;
}

bool operator!=(flatten const& a, flatten const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, flatten const& value) {
    return out << value.kind() << "()";
}

bool flatten::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<flatten>(other);
}

std::ostream& flatten::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
