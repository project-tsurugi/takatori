#include <takatori/value/decimal.h>

#include <string_view>

#include <cstring>

namespace takatori::value {

decimal::decimal(entity_type value) noexcept
    : entity_(value)
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
    auto&& d = entity_.entity();
    std::string_view p {
            reinterpret_cast<std::string_view::const_pointer>(&d), // NOLINT
            sizeof(d),
    };
    return std::hash<std::string_view>{}(p);
}

std::ostream& decimal::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(decimal const& a, decimal const& b) noexcept {
    auto&& d1 = a.get().entity();
    auto&& d2 = b.get().entity();
    return std::memcmp(&d1, &d2, sizeof(decltype(d1))) == 0;
}

bool operator!=(decimal const& a, decimal const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, decimal const& value) {
    return out << value.get();
}

} // namespace takatori::value
