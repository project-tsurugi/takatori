#include "takatori/scalar/expression.h"

namespace takatori::scalar {

util::optional_ptr<expression> expression::parent_expression() noexcept {
    util::optional_ptr result { parent_element() };
    return util::dynamic_pointer_cast<expression>(result);
}

util::optional_ptr<expression const> expression::parent_expression() const noexcept {
    util::optional_ptr result { parent_element() };
    return util::dynamic_pointer_cast<expression>(result);
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
