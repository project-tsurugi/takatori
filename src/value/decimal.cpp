#include "takatori/value/decimal.h"

#include <string_view>

#include <cstring>

namespace takatori::value {

decimal::decimal(entity_type value) noexcept
    : entity_(value)
{}

value_kind decimal::kind() const noexcept {
    return tag;
}

decimal* decimal::clone(util::object_creator creator) const& {
    return creator.create_object<decimal>(entity_);
}

decimal* decimal::clone(util::object_creator creator) && {
    return creator.create_object<decimal>(entity_);
}

decimal::view_type decimal::get() const noexcept {
    return entity_;
}

decimal::operator view_type() const noexcept {
    return get();
}

bool operator==(decimal const& a, decimal const& b) noexcept {
    auto&& d1 = a.entity_.entity();
    auto&& d2 = b.entity_.entity();
    return std::memcmp(&d1, &d2, sizeof(decltype(d1))) == 0;
}

bool operator!=(decimal const& a, decimal const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, decimal const& value) {
    return out << value.get();
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

} // namespace takatori::value
