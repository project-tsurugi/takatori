#include <takatori/relation/step/intersection.h>

#include <takatori/util/downcast.h>

namespace takatori::relation::step {

intersection::intersection(util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
{}

intersection::intersection(intersection const&, util::object_creator creator)
    : intersection(creator)
{}

intersection::intersection(intersection&&, util::object_creator creator)
    : intersection(creator)
{}

expression_kind intersection::kind() const noexcept {
    return tag;
}

intersection* intersection::clone(util::object_creator creator) const& {
    return creator.create_object<intersection>(*this, creator);
}

intersection* intersection::clone(util::object_creator creator)&& {
    return creator.create_object<intersection>(std::move(*this), creator);
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
