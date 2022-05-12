#include <takatori/value/time_point.h>

namespace takatori::value {

time_point::time_point(datetime::date date, datetime::time_of_day time) noexcept
    : entity_(date, time)
{}

time_point::time_point(
        std::int32_t year,
        std::uint32_t month,
        std::uint32_t day,
        std::uint32_t hour,
        std::uint32_t minute,
        std::uint32_t second,
        datetime::time_of_day::time_unit subsecond)
    : time_point(
        datetime::date { year, month, day },
        datetime::time_of_day { hour, minute, second, subsecond })
{}

value_kind time_point::kind() const noexcept {
    return tag;
}

time_point* time_point::clone() const& {
     return new time_point(entity_); // NOLINT
}

time_point* time_point::clone() && {
     return new time_point(entity_); // NOLINT
}

std::ostream& operator<<(std::ostream& out, time_point const& value) {
    return out << value.get();
}

bool time_point::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<time_point>(other);
}

std::size_t time_point::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& time_point::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::value
