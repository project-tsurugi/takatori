#include <takatori/scalar/expression.h>

#include <takatori/util/downcast.h>

namespace takatori::scalar {

expression::parent_type* expression::parent_element() noexcept {
    return parent_;
}

expression::parent_type const* expression::parent_element() const noexcept {
    return parent_;
}

void expression::parent_element(expression::parent_type* parent) noexcept {
    parent_ = parent;
}

util::optional_ptr<expression> expression::parent_expression() noexcept {
    return util::optional_ptr { util::downcast<expression>(parent_element()) };
}

util::optional_ptr<expression const> expression::parent_expression() const noexcept {
    return util::optional_ptr { util::downcast<expression>(parent_element()) };
}

document::region& expression::region() noexcept {
    return region_;
}

document::region const& expression::region() const noexcept {
    return region_;
}

bool operator==(expression const& a, expression const& b) noexcept {
    return a.equals(b);
}

bool operator!=(expression const& a, expression const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, expression const& value) {
    return value.print_to(out);
}

} // namespace takatori::scalar
