#include <takatori/type/extension.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

type_kind extension::kind() const noexcept {
    return tag;
}

bool operator==(extension const& a, extension const& b) noexcept {
    return a.equals(b);
}

bool operator!=(extension const& a, extension const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, extension const& value) {
    return value.print_to(out);
}

bool extension::equals(data const& other) const noexcept {
    return tag == other.kind() && equals(util::unsafe_downcast<extension>(other));
}

} // namespace takatori::type
