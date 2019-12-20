#include <takatori/value/datetime_interval.h>

namespace takatori::value {

value_kind datetime_interval::kind() const noexcept {
    return tag;
}

datetime_interval* datetime_interval::clone(util::object_creator creator) const& {
    return creator.create_object<datetime_interval>(entity_);
}

datetime_interval* datetime_interval::clone(util::object_creator creator) && {
    return creator.create_object<datetime_interval>(entity_);
}

std::ostream& operator<<(std::ostream& out, datetime_interval const& value) {
    return out << value.get();
}

bool datetime_interval::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<datetime_interval>(other);
}

std::size_t datetime_interval::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& datetime_interval::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::value
