#include "takatori/type/declared.h"

#include "takatori/util/downcast.h"

namespace takatori::type {

declared::declared(descriptor::declared_type binding) noexcept
    : binding_(std::move(binding))
{}

type_kind declared::kind() const noexcept {
    return tag;
}

declared* declared::clone(util::object_creator creator) const& {
    return creator.create_object<declared>(binding_);
}

declared* declared::clone(util::object_creator creator) && {
    return creator.create_object<declared>(std::move(binding_));
}

descriptor::declared_type const& declared::binding() const noexcept {
    return binding_;
}

bool operator==(declared const& a, declared const& b) noexcept {
    return a.binding_ == b.binding_;
}

bool operator!=(declared const& a, declared const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, declared const& value) {
    return out << "declared(" << value.binding().optional_entity() << ")";
}

bool declared::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<declared>(other);
}

std::size_t
declared::hash() const noexcept {
    return std::hash<decltype(binding_)>{}(binding_);
}

std::ostream& declared::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
