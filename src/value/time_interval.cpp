#include "takatori/value/time_interval.h"

namespace takatori::value {

value_kind time_interval::kind() const noexcept {
    return tag;
}

time_interval* time_interval::clone(util::object_creator creator) const& {
    return creator.create_object<time_interval>(entity_);
}

time_interval* time_interval::clone(util::object_creator creator) && {
    return creator.create_object<time_interval>(entity_);
}

std::ostream& operator<<(std::ostream& out, time_interval const& value) {
    return out << value.get();
}

bool time_interval::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<time_interval>(other);
}

std::size_t time_interval::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& time_interval::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::value
