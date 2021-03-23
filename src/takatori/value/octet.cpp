#include <takatori/value/octet.h>

namespace takatori::value {

octet::octet(entity_type value) noexcept :
    entity_ { std::move(value) }
{}

value_kind octet::kind() const noexcept {
    return tag;
}

octet* octet::clone(util::object_creator creator) const& {
    return creator.create_object<octet>(entity_, creator.allocator());
}

octet* octet::clone(util::object_creator creator) && {
    if (creator.is_compatible(entity_.get_allocator())) {
        return creator.create_object<octet>(std::move(entity_));
    }
    return creator.create_object<octet>(entity_, creator.allocator());
}

octet::view_type octet::get() const noexcept {
    return entity_;
}

octet::operator view_type() const noexcept {
    return get();
}

bool octet::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<octet>(other);
}

std::size_t octet::hash() const noexcept {
    return std::hash<view_type>{}(get());
}

std::ostream& octet::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(octet const& a, octet const& b) noexcept {
    return a.get() == b.get();
}

bool operator!=(octet const& a, octet const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, octet const& value) {
    return out << "octet(" << value.get() << ")";
}

} // namespace takatori::value
