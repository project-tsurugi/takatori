#include <takatori/relation/details/apply_column.h>

#include <takatori/util/hash.h>

namespace takatori::relation::details {

apply_column::apply_column(position_type position, variable_type variable) noexcept:
    position_ { position },
    variable_ { std::move(variable) }
{}

apply_column::position_type apply_column::position() const noexcept {
    return position_;
}

apply_column& apply_column::position(position_type position) noexcept {
    position_ = position;
    return *this;
}

apply_column::variable_type& apply_column::variable() noexcept {
    return variable_;
}

apply_column::variable_type const& apply_column::variable() const noexcept {
    return variable_;
}

bool operator==(apply_column const& a, apply_column const& b) noexcept {
    return a.position() == b.position()
        && a.variable() == b.variable();
}

bool operator!=(apply_column const& a, apply_column const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, apply_column const& value) {
    return out << "column("
               << value.position() << ": "
               << value.variable() << ")";
}

} // namespace takatori::relation::details

std::size_t std::hash<takatori::relation::details::apply_column>::operator()(
        takatori::relation::details::apply_column const& value) const noexcept {
    return ::takatori::util::hash(value.position(), value.variable());
}
