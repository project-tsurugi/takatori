#include "takatori/value/time_point.h"

namespace takatori::value {

value_kind time_point::kind() const noexcept {
    return tag;
}

time_point* time_point::clone(util::object_creator creator) const& {
    return creator.create_object<time_point>(entity_);
}

time_point* time_point::clone(util::object_creator creator) && {
    return creator.create_object<time_point>(entity_);
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
