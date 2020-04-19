#include <takatori/value/primitive.h>

#include <takatori/util/print_support.h>

namespace takatori::value {

std::ostream& operator<<(std::ostream& out, boolean const& value) {
    return out << "boolean("
            << util::print_support { value.get() } << ")";
}

} // namespace takatori::value
