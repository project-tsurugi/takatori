#include <takatori/value/octet.h>

namespace takatori::value {

octet::octet(entity_type value) noexcept :
    entity_ { std::move(value) }
{}

value_kind octet::kind() const noexcept {
    return tag;
}

octet* octet::clone(util::object_creator creator) const& {
    return creator.create_object<octet>(entity_, creator.allocator());
}

octet* octet::clone(util::object_creator creator) && {
    if (creator.is_compatible(entity_.get_allocator())) {
        return creator.create_object<octet>(std::move(entity_));
    }
    return creator.create_object<octet>(entity_, creator.allocator());
}

octet::view_type octet::get() const noexcept {
    return entity_;
}

octet::operator view_type() const noexcept {
    return get();
}

bool octet::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<octet>(other);
}

std::size_t octet::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& octet::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(octet const& a, octet const& b) noexcept {
    return a.get() == b.get();
}

bool operator!=(octet const& a, octet const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, octet const& value) {
    static constexpr std::array<char, 16> hex {
            '0', '1', '2', '3',
            '4', '5', '6', '7',
            '8', '9', 'a', 'b',
            'c', 'd', 'e', 'f',
    };
    out << "octet(";
    for (auto c : value.get()) {
        std::size_t v = static_cast<unsigned char>(c);
        auto hi = static_cast<std::size_t>((v >> 4U) & 0x0fU);
        auto lo = static_cast<std::size_t>(v & 0x0fU);
        out << hex[hi] << hex[lo]; // NOLINT
    }
    return out << ")";
}

} // namespace takatori::value
