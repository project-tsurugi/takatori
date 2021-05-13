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

} // namespace takatori::value
