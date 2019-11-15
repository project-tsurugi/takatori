#include "takatori/value/date.h"

namespace takatori::value {

date::date(std::uint32_t year, std::uint32_t month, std::uint32_t day) noexcept
    : entity_(year, month, day)
{}

value_kind date::kind() const noexcept {
    return tag;
}

date* date::clone(util::object_creator creator) const& {
    return creator.create_object<date>(entity_);
}

date* date::clone(util::object_creator creator) && {
    return creator.create_object<date>(entity_);
}

std::ostream& operator<<(std::ostream& out, date const& value) {
    return out << value.get();
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

} // namespace takatori::value
