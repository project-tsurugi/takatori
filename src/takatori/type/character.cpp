#include <takatori/type/character.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

type_kind character::kind() const noexcept {
    return tag;
}

character* character::clone() const& {
     return new character(varying_t { varying_ }, length_); // NOLINT
}

character* character::clone() && {
     return new character(varying_t { varying_ }, std::move(length_)); // NOLINT
}

bool operator==(character const& a, character const& b) noexcept {
    return a.varying_ == b.varying_
        && a.length_ == b.length_;
}

bool operator!=(character const& a, character const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, character const& value) {
    out << "character";
    if (value.varying_) {
        out << "-varying";
    }
    out << "(";
    if (value.length_) {
        out << value.length_.value();
    } else {
        out << "*";
    }
    out << ")";
    return out;
}

bool character::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<character>(other);
}

std::size_t
character::hash() const noexcept {
    return (varying_ ? 0ULL : 1ULL) * 31ULL
           + static_cast<std::size_t>(length_.value_or(-1));
}

std::ostream& character::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
