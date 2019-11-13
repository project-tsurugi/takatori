#include "takatori/chrono/time_point.h"

namespace takatori::chrono {

std::ostream& operator<<(std::ostream& out, time_point const& value) {
    out << "time_point(";
    value.print_body(out);
    out << ")";
    return out;
}

std::ostream& time_point::print_body(std::ostream& out) const {
    date_.print_body(out);
    time_.print_body(out);
    return out;
}

} // namespace takatori::chrono
