#include "takatori/scalar/immediate.h"

#include "takatori/util/downcast.h"

namespace takatori::scalar {

immediate::immediate(
        descriptor::value_descriptor value,
        descriptor::type_descriptor type) noexcept
    : value_(std::move(value))
    , type_(std::move(type))
{}

immediate::immediate(immediate const& other, util::object_creator) noexcept
    : immediate(other.value_, other.type_)
{}

immediate::immediate(immediate&& other, util::object_creator) noexcept
    : immediate(std::move(other.value_), std::move(other.type_))
{}

expression::parent_type* immediate::parent_element() noexcept {
    return parent_;
}

expression::parent_type const* immediate::parent_element() const noexcept {
    return parent_;
}

void immediate::parent_element(expression::parent_type* parent) noexcept {
    parent_ = parent;
}

expression_kind immediate::kind() const noexcept {
    return tag;
}

immediate* immediate::clone(util::object_creator creator) const& {
    return creator.create_object<immediate>(*this, creator);
}

immediate* immediate::clone(util::object_creator creator) && {
    return creator.create_object<immediate>(std::move(*this), creator);
}

descriptor::value_descriptor const& immediate::value() const noexcept {
    return value_;
}

immediate& immediate::value(descriptor::value_descriptor value) noexcept {
    value_ = std::move(value);
    return *this;
}

descriptor::type_descriptor const& immediate::type() const noexcept {
    return type_;
}

immediate& immediate::type(descriptor::type_descriptor type) noexcept {
    type_ = std::move(type);
    return *this;
}

bool operator==(immediate const& a, immediate const& b) noexcept {
    return a.value() == b.value()
        && b.type() == b.type();
}

bool operator!=(immediate const& a, immediate const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, immediate const& value) {
    return out << "immediate("
               << "value=" << value.value() << ", "
               << "type=" << value.type() << ")";
}

bool immediate::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<immediate>(other);
}

std::ostream& immediate::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
