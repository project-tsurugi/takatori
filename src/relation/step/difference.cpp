#include <takatori/relation/step/difference.h>

#include <takatori/util/downcast.h>

namespace takatori::relation::step {

difference::difference(util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
{}

difference::difference(difference const&, util::object_creator creator)
    : difference(creator)
{}

difference::difference(difference&&, util::object_creator creator)
    : difference(creator)
{}

expression_kind difference::kind() const noexcept {
    return tag;
}

difference* difference::clone(util::object_creator creator) const& {
    return creator.create_object<difference>(*this, creator);
}

difference* difference::clone(util::object_creator creator)&& {
    return creator.create_object<difference>(std::move(*this), creator);
}

util::sequence_view<difference::input_port_type> difference::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<difference::input_port_type const> difference::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<difference::output_port_type> difference::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<difference::output_port_type const> difference::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

difference::input_port_type& difference::input() noexcept {
    return input_;
}

difference::input_port_type const& difference::input() const noexcept {
    return input_;
}

difference::output_port_type& difference::output() noexcept {
    return output_;
}

difference::output_port_type const& difference::output() const noexcept {
    return output_;
}

bool operator==(difference const&, difference const&) noexcept {
    return true;
}

bool operator!=(difference const& a, difference const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, difference const& value) {
    return out << value.kind() << "()";
}

bool difference::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<difference>(other);
}

std::ostream& difference::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
