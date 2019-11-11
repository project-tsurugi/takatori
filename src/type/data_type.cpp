#include "takatori/type/data_type.h"

namespace takatori::type {

bool operator==(data_type const& a, data_type const& b) noexcept {
    return a.equals(b);
}

bool operator!=(data_type const& a, data_type const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, data_type const& value) {
    return value.print_to(out);
}

} // namespace takatori::type
