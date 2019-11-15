#include "takatori/datetime/time_of_day.h"

#include "../util/iomanip_util.h"

namespace takatori::datetime {

std::ostream& operator<<(std::ostream& out, time_of_day value) {
    out << "time_of_day(";
    value.print_body(out);
    out << ")";
    return out;
}

std::ostream& time_of_day::print_body(std::ostream& out) const {
    util::instant_fill fill { out, '0' };
    return out << std::setw(2) << hour() << ":"
               << std::setw(2) << minute() << ":"
               << std::setw(2) << second() << "."
               << std::setw(9) << subsecond().count();
}

} // namespace takatori::datetime
