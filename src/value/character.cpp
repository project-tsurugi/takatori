#include <takatori/value/character.h>

namespace takatori::value {

character::character(entity_type value) noexcept
    : entity_(std::move(value))
{}

value_kind character::kind() const noexcept {
    return tag;
}

character* character::clone(util::object_creator creator) const& {
    return creator.create_object<character>(entity_, creator.allocator<char>());
}

character* character::clone(util::object_creator creator) && {
    if (creator.is_compatible(entity_.get_allocator())) {
        return creator.create_object<character>(std::move(entity_));
    }
    return creator.create_object<character>(entity_, creator.allocator<char>());
}

character::view_type character::get() const noexcept {
    return entity_;
}

character::operator view_type() const noexcept {
    return get();
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

bool character::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<character>(other);
}

std::size_t character::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& character::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::value
