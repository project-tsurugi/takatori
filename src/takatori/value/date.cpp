#include <takatori/value/date.h>

namespace takatori::value {

date::date(std::uint32_t year, std::uint32_t month, std::uint32_t day) noexcept
    : entity_(year, month, day)
{}

value_kind date::kind() const noexcept {
    return tag;
}

date* date::clone() const& {
     return new date(entity_); // NOLINT
}

date* date::clone() && {
     return new date(entity_); // NOLINT
}

bool date::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<date>(other);
}

std::size_t date::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& date::print_to(std::ostream& out) const {
    return out << *this;
}

std::ostream& operator<<(std::ostream& out, date const& value) {
    return out << value.get();
}

} // namespace takatori::value
