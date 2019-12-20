#include <takatori/type/time_of_day.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

time_of_day::time_of_day(std::optional<datetime::time_zone> time_zone) noexcept
    : time_zone_(std::move(time_zone))
{}

type_kind time_of_day::kind() const noexcept {
    return tag;
}

time_of_day* time_of_day::clone(util::object_creator creator) const& {
    return creator.create_object<time_of_day>(time_zone_);
}

time_of_day* time_of_day::clone(util::object_creator creator) && {
    return creator.create_object<time_of_day>(std::move(time_zone_));
}

std::optional<datetime::time_zone> const& time_of_day::time_zone() const noexcept {
    return time_zone_;
}

bool operator==(time_of_day const& a, time_of_day const& b) noexcept {
    return a.time_zone_ == b.time_zone_;
}

bool operator!=(time_of_day const& a, time_of_day const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, time_of_day const& value) {
    out << "time_of_day(";
    if (value.time_zone_) {
        out << "time_zone=" << value.time_zone_.value();
    } else {
        out << "time_zone=N/A";
    }
    out << ")";
    return out;
}

bool time_of_day::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<time_of_day>(other);
}

std::size_t
time_of_day::hash() const noexcept {
    return std::hash<decltype(time_zone_)>{}(time_zone_);
}

std::ostream& time_of_day::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
