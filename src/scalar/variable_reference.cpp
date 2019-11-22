#include "takatori/scalar/variable_reference.h"

#include "takatori/util/downcast.h"

namespace takatori::scalar {

variable_reference::variable_reference(descriptor::variable variable) noexcept
    : variable_(std::move(variable))
{}

variable_reference::variable_reference(variable_reference const& other, util::object_creator) noexcept
    : variable_reference(other.variable_)
{}

variable_reference::variable_reference(variable_reference&& other, util::object_creator) noexcept
    : variable_reference(std::move(other.variable_))
{}

expression_kind variable_reference::kind() const noexcept {
    return tag;
}

variable_reference* variable_reference::clone(util::object_creator creator) const& {
    return creator.create_object<variable_reference>(*this, creator);
}

variable_reference* variable_reference::clone(util::object_creator creator) && {
    return creator.create_object<variable_reference>(std::move(*this), creator);
}

descriptor::variable const& variable_reference::variable() const noexcept {
    return variable_;
}

variable_reference& variable_reference::variable(descriptor::variable variable) noexcept {
    variable_ = std::move(variable);
    return *this;
}

bool operator==(variable_reference const& a, variable_reference const& b) noexcept {
    return a.variable() == b.variable();
}

bool operator!=(variable_reference const& a, variable_reference const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, variable_reference const& value) {
    return out << value.kind() << "(" << value.variable() << ")";
}

bool variable_reference::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<variable_reference>(other);
}

std::ostream& variable_reference::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
