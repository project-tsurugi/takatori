#include <takatori/scalar/variable_reference.h>

#include <takatori/util/downcast.h>

namespace takatori::scalar {

variable_reference::variable_reference(descriptor::variable variable) noexcept
    : variable_(std::move(variable))
{}

variable_reference::variable_reference(util::clone_tag_t, variable_reference const& other) noexcept
    : variable_reference(other.variable_)
{}

variable_reference::variable_reference(util::clone_tag_t, variable_reference&& other) noexcept
    : variable_reference(std::move(other.variable_))
{}

expression_kind variable_reference::kind() const noexcept {
    return tag;
}

variable_reference* variable_reference::clone() const& {
    return new variable_reference(util::clone_tag, *this); // NOLINT
}

variable_reference* variable_reference::clone() && {
    return new variable_reference(util::clone_tag, std::move(*this)); // NOLINT;
}

descriptor::variable& variable_reference::variable() noexcept {
    return variable_;
}

descriptor::variable const& variable_reference::variable() const noexcept {
    return variable_;
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
