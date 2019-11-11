#include "takatori/type/time_point.h"

#include "takatori/util/downcast.h"

namespace takatori::type {

time_point::time_point(std::optional<descriptor::time_zone> time_zone) noexcept
    : time_zone_(std::move(time_zone))
{}

data_type_kind time_point::kind() const noexcept {
    return tag;
}

time_point* time_point::clone(util::object_creator creator) const& {
    return creator.create_object<time_point>(time_zone_);
}

time_point* time_point::clone(util::object_creator creator) && {
    return creator.create_object<time_point>(std::move(time_zone_));
}

std::optional<descriptor::time_zone> const& time_point::time_zone() const noexcept {
    return time_zone_;
}

bool operator==(time_point const& a, time_point const& b) noexcept {
    // FIXME: comparison
    return a.time_zone_ == b.time_zone_;
}

bool operator!=(time_point const& a, time_point const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, time_point const& value) {
    out << "time_point(";
    if (value.time_zone_) {
        // FIXME: printing
        out << "time_zone=" << value.time_zone_.value();
    } else {
        out << "time_zone=N/A";
    }
    out << ")";
    return out;
}

bool time_point::equals(data_type const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<time_point>(other);
}

std::size_t
time_point::hash() const noexcept {
    if (time_zone_) {
        // FIXME: comparison
        return std::hash<decltype(time_zone_)>{}(time_zone_);
    }
    return 0;
}

std::ostream& time_point::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
