#include "takatori/value/time_of_day.h"

namespace takatori::value {

time_of_day::time_of_day(
        std::uint32_t hour,
        std::uint32_t minute,
        std::uint32_t second,
        datetime::time_of_day::time_unit subsecond) noexcept
    : entity_(hour, minute, second, subsecond)
{}

value_kind time_of_day::kind() const noexcept {
    return tag;
}

time_of_day* time_of_day::clone(util::object_creator creator) const& {
    return creator.create_object<time_of_day>(entity_);
}

time_of_day* time_of_day::clone(util::object_creator creator) && {
    return creator.create_object<time_of_day>(entity_);
}

std::ostream& operator<<(std::ostream& out, time_of_day const& value) {
    return out << value.get();
}

bool time_of_day::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<time_of_day>(other);
}

std::size_t time_of_day::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& time_of_day::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::value
