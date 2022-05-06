#include <takatori/util/buffer_view.h>

namespace takatori::util {

std::ostream& operator<<(std::ostream& out, buffer_view value) {
    return out << "buffer_view("
        << "data=" << value.data() << ", "
        << "size=" << value.size() << ")";
}

} // namespace takatori::util
