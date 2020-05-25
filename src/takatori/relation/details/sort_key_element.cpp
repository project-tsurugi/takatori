#include <takatori/relation/details/sort_key_element.h>

#include <utility>

#include <takatori/util/hash.h>

namespace takatori::relation::details {

sort_key_element::sort_key_element(descriptor::variable variable, sort_direction direction) noexcept
    : variable_(std::move(variable))
    , direction_(direction)
{}

descriptor::variable& sort_key_element::variable() noexcept {
    return variable_;
}

descriptor::variable const& sort_key_element::variable() const noexcept {
    return variable_;
}

sort_direction const& sort_key_element::direction() const noexcept {
    return direction_;
}

sort_key_element& sort_key_element::direction(sort_direction direction) noexcept {
    direction_ = direction;
    return *this;
}

bool operator==(sort_key_element const& a, sort_key_element const& b) noexcept {
    return a.variable() == b.variable()
        && a.direction() == b.direction();
}

bool operator!=(sort_key_element const& a, sort_key_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, sort_key_element const& value) {
    return out << "sort_key" << "("
               << "variable=" << value.variable() << ", "
               << "direction=" << value.direction() << ")";
}

} // namespace takatori::relation::details

std::size_t std::hash<takatori::relation::details::sort_key_element>::operator()(
        takatori::relation::details::sort_key_element const& value) const noexcept {
    return takatori::util::hash(value.variable(), value.direction());
}
