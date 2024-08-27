#include <takatori/value/character.h>

namespace takatori::value {

character::character(entity_type value) noexcept
    : entity_(std::move(value))
{}

value_kind character::kind() const noexcept {
    return tag;
}

character* character::clone() const& {
     return new character(entity_); // NOLINT
}

character* character::clone() && {
     return new character(std::move(entity_)); // NOLINT
}

character::view_type character::get() const noexcept {
    return entity_;
}

character::operator view_type() const noexcept {
    return get();
}

bool character::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<character>(other);
}

std::size_t character::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& character::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(character const& a, character const& b) noexcept {
    return a.get() == b.get();
}

bool operator!=(character const& a, character const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, character const& value) {
    return out << "character(" << value.get() << ")";
}

std::string_view truncate_utf8(std::string_view str, std::size_t size) {
    if (str.size() <= size) {
        return str;
    }

    constexpr unsigned char mask1 = 0b1000'0000;
    constexpr unsigned char mask2 = 0b1100'0000;
    constexpr unsigned char mask3 = 0b1110'0000;
    constexpr unsigned char mask4 = 0b1111'0000;
    constexpr unsigned char mask5 = 0b1111'1000;

    constexpr unsigned char body2 = 0b1000'0000;
    constexpr unsigned char head1_1 = 0b0000'0000;
    constexpr unsigned char head3_2 = 0b1100'0000;
    constexpr unsigned char head4_3 = 0b1110'0000;
    constexpr unsigned char head5_4 = 0b1111'0000;

    constexpr std::size_t max_sequence_size = 4;

    // first, we find for the head of the last UTF-8 sequence
    // we don't need to search for the 4-byte UTF-8 sequence.
    for (std::size_t backward_offset = 1; backward_offset <= max_sequence_size - 1; ++backward_offset) {
        if (backward_offset > size) {
            // no head of UTF-8 sequences found
            break;
        }
        auto position = size - backward_offset;
        auto byte = static_cast<unsigned char>(str[position]);

        if ((byte & mask2) == body2) {
            // found non-head UTF-8 element, continue to backward.
            continue;
        }

        if ((byte & mask1) == head1_1) {
            // found 1-byte UTF-8 sequence
            return str.substr(0, position + 1);
        }
        if ((byte & mask3) == head3_2) {
            // found 2-byte UTF-8 sequence
            if (backward_offset >= 2) {
                return str.substr(0, position + 2);
            }
            // truncate the current 2-byte UTF-8 sequence
            return str.substr(0, position);
        }
        if ((byte & mask4) == head4_3) {
            // found 3-byte UTF-8 sequence
            if (backward_offset >= 3) {
                return str.substr(0, position + 3);
            }
            // truncate the current 3-byte UTF-8 sequence
            return str.substr(0, position);
        }
        if ((byte & mask5) == head5_4) {
            // found 4-byte UTF-8 sequence
            // truncate the current 4-byte UTF-8 sequence
            return str.substr(0, position);
        }
        // We not found a head of UTF-8 sequence.
        // It because one of the following reasons:
        // 1. the input string is a well-formed UTF-8 string, and the last one is a 4-byte UTF-8 sequence.
        // 2. the input string is not a well-formed UTF-8 string.
        // Anyway, we can obtain the expected result by trim to the originally specified size.
        break;
    }

    // if the head of the last UTF-8 sequence is not found, we just return the truncated string
    return str.substr(0, size);
}

} // namespace takatori::value
