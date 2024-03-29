#include <takatori/value/decimal.h>

#include <string_view>

#include <cstring>

namespace takatori::value {

decimal::decimal(entity_type value) noexcept :
    entity_ { value }
{}

decimal::decimal(char const* text) :
    entity_ { text }
{}

value_kind decimal::kind() const noexcept {
    return tag;
}

decimal* decimal::clone() const& {
     return new decimal(entity_); // NOLINT
}

decimal* decimal::clone() && {
     return new decimal(entity_); // NOLINT
}

decimal::view_type decimal::get() const noexcept {
    return entity_;
}

decimal::operator view_type() const noexcept {
    return get();
}

bool decimal::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<decimal>(other);
}

std::size_t decimal::hash() const noexcept {
    return std::hash<entity_type> {}(get());
}

std::ostream& decimal::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(decimal const& a, decimal const& b) noexcept {
    return a.get() == b.get();
}

bool operator!=(decimal const& a, decimal const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, decimal const& value) {
    return out << value.get();
}

} // namespace takatori::value
