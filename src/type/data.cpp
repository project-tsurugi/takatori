#include <takatori/type/data.h>

namespace takatori::type {

bool operator==(data const& a, data const& b) noexcept {
    return a.equals(b);
}

bool operator!=(data const& a, data const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, data const& value) {
    return value.print_to(out);
}

} // namespace takatori::type
