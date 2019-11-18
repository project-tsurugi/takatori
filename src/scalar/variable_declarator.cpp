#include "takatori/scalar/variable_declarator.h"

#include "takatori/tree/tree_element_util.h"
#include "tree/tree_element_forward.h"

#include "takatori/util/clonable.h"

namespace takatori::scalar {

variable_declarator::variable_declarator(
        descriptor::variable variable,
        util::unique_object_ptr<expression> value) noexcept
    : variable_(std::move(variable))
    , value_(std::move(value))
{}

variable_declarator::variable_declarator(
        descriptor::variable variable,
        expression&& value) noexcept
    : variable_declarator(
            std::move(variable),
            util::clone_unique(std::move(value)))
{}

variable_declarator::variable_declarator(
        variable_declarator const& other,
        util::object_creator creator)
    : variable_declarator(
            other.variable_,
            tree::forward(creator, other.value_))
{}

variable_declarator::variable_declarator(
        variable_declarator&& other,
        util::object_creator creator)
    : variable_declarator(
            std::move(other.variable_),
            tree::forward(creator, std::move(other.value_)))
{}

descriptor::variable const& variable_declarator::variable() const noexcept {
    return variable_;
}

variable_declarator& variable_declarator::variable(descriptor::variable variable) noexcept {
    variable_ = std::move(variable);
    return *this;
}

expression& variable_declarator::value() noexcept {
    return *value_;
}

expression const& variable_declarator::value() const noexcept {
    return *value_;
}

util::optional_ptr<expression> variable_declarator::optional_value() noexcept {
    return util::optional_ptr { value_.get() };
}

util::optional_ptr<expression const> variable_declarator::optional_value() const noexcept {
    return util::optional_ptr { value_.get() };
}

util::unique_object_ptr<expression> variable_declarator::release_value() noexcept {
    return tree::release_element(std::move(value_));
}

util::unique_object_ptr<expression>& variable_declarator::internal_value() noexcept {
    return value_;
}

bool variable_declarator::equals(variable_declarator const& other) const noexcept {
    return variable() == other.variable()
           && optional_value() == other.optional_value();
}

std::ostream& variable_declarator::print_to(std::ostream& out) const {
    return out << "("
               << "variable=" << variable() << ", "
               << "value=" << optional_value() << ")";
}

bool operator==(variable_declarator const& a, variable_declarator const& b) noexcept {
    return a.equals(b);
}

bool operator!=(variable_declarator const& a, variable_declarator const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, variable_declarator const& value) {
    out << "variable_declarator";
    return value.print_to(out);
}

} // namespace takatori::scalar
