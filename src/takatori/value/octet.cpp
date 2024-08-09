#include <takatori/value/octet.h>

namespace takatori::value {

octet::octet(entity_type value) noexcept :
    entity_ { std::move(value) }
{}

value_kind octet::kind() const noexcept {
    return tag;
}

octet* octet::clone() const& {
     return new octet(entity_); // NOLINT
}

octet* octet::clone() && {
     return new octet(std::move(entity_)); // NOLINT
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

util::either<std::string, std::size_t> parse_octet(std::string_view literal, std::string& buffer) {
    using std::string_literals::operator""s;
    std::size_t hex_digit_count = 0;
    for (auto c : literal) {
        if (c == ' ') {
            continue;
        }
        if (('0' <= c && c <= '9') || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f')) {
            ++hex_digit_count;
            continue;
        }
        return "invalid character in octet string"s;
    }
    if (hex_digit_count % 2 != 0) {
        return "unexpected end of octet string (odd hex digits)"s;
    }
    std::size_t octet_count = hex_digit_count / 2;

    std::size_t offset { buffer.size() };
    buffer.resize(offset + octet_count);
    std::uint8_t octet {};
    bool high = true;
    for (auto c : literal) {
        if (c == ' ') {
            continue;
        }
        if ('0' <= c && c <= '9') {
            octet |= static_cast<std::uint8_t>(c - '0');
        } else if ('A' <= c && c <= 'F') {
            octet |= static_cast<std::uint8_t>(c - 'A' + 10);
        } else if ('a' <= c && c <= 'f') {
            octet |= static_cast<std::uint8_t>(c - 'a' + 10);
        }
        if (high) {
            high = false;
            octet = static_cast<std::uint8_t>(octet << 4U);
            continue;
        }
        high = true;
        buffer[offset] = static_cast<std::string::value_type>(octet);
        octet = 0U;
        ++offset;
    }
    return octet_count;
}

} // namespace takatori::value
