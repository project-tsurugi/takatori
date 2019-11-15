#include "takatori/datetime/datetime_interval.h"

namespace takatori::datetime {

std::ostream& datetime_interval::print_to(std::ostream& out) {
    date_.print_to(out);
    out << " ";
    time_.print_to(out);
    return out;
}

std::ostream& operator<<(std::ostream& out, datetime_interval value) {
    out << "datetime_interval(";
    value.print_to(out);
    out << ")";
    return out;
}

} // namespace takatori::datetime
