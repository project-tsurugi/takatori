#include <takatori/type/time_point.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

type_kind time_point::kind() const noexcept {
    return tag;
}

time_point* time_point::clone() const& {
    return new time_point(with_time_zone_t { with_time_zone_ }); // NOLINT
}

time_point* time_point::clone() && {
    return new time_point(with_time_zone_t { with_time_zone_ }); // NOLINT
}

bool operator==(time_point const& a, time_point const& b) noexcept {
    return a.with_time_zone_ == b.with_time_zone_;
}

bool operator!=(time_point const& a, time_point const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, time_point const& value) {
    return out << "time_point(" << with_time_zone_t { value.with_time_zone_ } << ")";
}

bool time_point::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<time_point>(other);
}

std::size_t
time_point::hash() const noexcept {
    return with_time_zone_ ? 1 : 0;
}

std::ostream& time_point::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
