#include "takatori/datetime/date_interval.h"

#include "../util/iomanip_util.h"

namespace takatori::datetime {

std::ostream& date_interval::print_to(std::ostream& out) {
    util::instant_fill fill { out, '0' };
    util::instant_showpos showpos { out };
    return out << year() << '/'
               << month() << '/'
               << day();
}

std::ostream& operator<<(std::ostream& out, date_interval value) {
    out << "date_interval(";
    value.print_to(out);
    out << ")";
    return out;
}

} // namespace takatori::datetime
