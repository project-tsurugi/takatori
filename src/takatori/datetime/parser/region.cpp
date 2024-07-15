#include "region.h"

namespace takatori::datetime::parser {

std::ostream& operator<<(std::ostream& out, region value) {
    return out
        << "[" << value.begin
        << "-" << value.end
        << "]";
}

} // namespace takatori::datetime::parser
