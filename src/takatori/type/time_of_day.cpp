#include <takatori/type/time_of_day.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

type_kind time_of_day::kind() const noexcept {
    return tag;
}

time_of_day* time_of_day::clone() const& {
     return new time_of_day(with_time_zone_t { with_time_zone_ }); // NOLINT
}

time_of_day* time_of_day::clone() && {
    return new time_of_day(with_time_zone_t { with_time_zone_ }); // NOLINT
}

bool operator==(time_of_day const& a, time_of_day const& b) noexcept {
    return a.with_time_zone_ == b.with_time_zone_;
}

bool operator!=(time_of_day const& a, time_of_day const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, time_of_day const& value) {
    return out << "time_of_day(" << with_time_zone_t { value.with_time_zone_ } << ")";
}

bool time_of_day::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<time_of_day>(other);
}

std::size_t
time_of_day::hash() const noexcept {
    return with_time_zone_ ? 1 : 0;
}

std::ostream& time_of_day::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
