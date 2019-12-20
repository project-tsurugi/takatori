#include <takatori/relation/step/distinct.h>

#include <takatori/util/downcast.h>

namespace takatori::relation::step {

distinct::distinct(util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
{}

distinct::distinct(distinct const&, util::object_creator creator)
    : distinct(creator)
{}

distinct::distinct(distinct&&, util::object_creator creator)
    : distinct(creator)
{}

expression_kind distinct::kind() const noexcept {
    return tag;
}

distinct* distinct::clone(util::object_creator creator) const& {
    return creator.create_object<distinct>(*this, creator);
}

distinct* distinct::clone(util::object_creator creator)&& {
    return creator.create_object<distinct>(std::move(*this), creator);
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

bool operator==(distinct const&, distinct const&) noexcept {
    return true;
}

bool operator!=(distinct const& a, distinct const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, distinct const& value) {
    return out << value.kind() << "()";
}

bool distinct::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<distinct>(other);
}

std::ostream& distinct::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
