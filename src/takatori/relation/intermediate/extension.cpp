#include <takatori/relation/intermediate/extension.h>

namespace takatori::relation::intermediate {

expression_kind extension::kind() const noexcept {
    return tag;
}

bool extension::equals(expression const& other) const noexcept {
    if (this == std::addressof(other)) {
        return true;
    }
    return tag == other.kind() && equals(util::unsafe_downcast<extension>(other));
}

bool operator==(extension const& a, extension const& b) noexcept {
    if (std::addressof(a) == std::addressof(b)) {
        return true;
    }
    return a.equals(b);
}

bool operator!=(extension const& a, extension const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, extension const& value) {
    return value.print_to(out);
}

} // namespace takatori::relation::intermediate
