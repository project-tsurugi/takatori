#include <takatori/type/time_point.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

time_point::time_point(std::optional<datetime::time_zone> time_zone) noexcept
    : time_zone_(std::move(time_zone))
{}

type_kind time_point::kind() const noexcept {
    return tag;
}

time_point* time_point::clone() const& {
     return new time_point(time_zone_); // NOLINT
}

time_point* time_point::clone() && {
     return new time_point(std::move(time_zone_)); // NOLINT
}

std::optional<datetime::time_zone> const& time_point::time_zone() const noexcept {
    return time_zone_;
}

bool operator==(time_point const& a, time_point const& b) noexcept {
    return a.time_zone_ == b.time_zone_;
}

bool operator!=(time_point const& a, time_point const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, time_point const& value) {
    out << "time_point(";
    if (value.time_zone_) {
        out << "time_zone=" << value.time_zone_.value();
    } else {
        out << "time_zone=N/A";
    }
    out << ")";
    return out;
}

bool time_point::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<time_point>(other);
}

std::size_t
time_point::hash() const noexcept {
    return std::hash<decltype(time_zone_)>{}(time_zone_);
}

std::ostream& time_point::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
