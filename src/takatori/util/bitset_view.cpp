#include <takatori/util/bitset_view.h>

namespace takatori::util {

std::ostream& operator<<(std::ostream& out, bitset_view value) {
    out << "bitset_view(";
    for (std::size_t i = 0, n = value.size(); i < n; ++i) {
        if (i != 0) {
            out << ", ";
        }
        out << (value[i] ? '1' : '0');
    }
    out << ")";
    return out;
}


} // namespace takatori::util
