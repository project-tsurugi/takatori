#include <takatori/value/data.h>

namespace takatori::value {

bool operator==(data const& a, data const& b) noexcept {
    return a.equals(b);
}

bool operator!=(data const& a, data const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, data const& value) {
    return value.print_to(out);
}

} // namespace takatori::value
