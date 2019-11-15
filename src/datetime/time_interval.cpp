#include "takatori/datetime/time_interval.h"

#include "../util/iomanip_util.h"

namespace takatori::datetime {

std::ostream& time_interval::print_to(std::ostream& out) {
    util::instant_fill fill { out, '0' };
    util::instant_showpos showpos { out };
    return out << day() << ':'
               << hour() << ':'
               << minute() << ':'
               << second() << '.'
               << std::setw(9) << std::noshowpos << std::abs(subsecond().count());
}

std::ostream& operator<<(std::ostream& out, time_interval value) {
    out << "time_interval(";
    value.print_to(out);
    out << ")";
    return out;
}

} // namespace takatori::datetime
