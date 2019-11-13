#include "takatori/chrono/date.h"

namespace takatori::chrono {

std::ostream& operator<<(std::ostream& out, date const& value) {
    out << "date(";
    value.print_body(out);
    out << ")";
    return out;
}

std::ostream& date::print_body(std::ostream& out) const {
    auto days = total_days();
    // FIXME: split to y-m-d
    out << days << "days since 1900-01-01";
    return out;
}

} // namespace takatori::chrono
