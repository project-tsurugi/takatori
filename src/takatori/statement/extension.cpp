#include <takatori/statement/extension.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

statement_kind extension::kind() const noexcept {
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

bool extension::equals(statement const& other) const noexcept {
    return tag == other.kind() && equals(util::unsafe_downcast<extension>(other));
}

} // namespace takatori::statement
