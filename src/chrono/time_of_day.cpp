#include "takatori/chrono/time_of_day.h"

#include "../util/iomanip_util.h"

namespace takatori::chrono {

std::ostream& operator<<(std::ostream& out, time_of_day const& value) {
    out << "time_of_day(";
    value.print_body(out);
    out << ")";
    return out;
}

std::ostream& time_of_day::print_body(std::ostream& out) const {
    auto secs = total_nanoseconds_ / 1'000'000'000;
    auto subsecs = total_nanoseconds_ % 1'000'000'000;

    util::instant_fill fill { out, '0' };
    out << std::setw(2) << (secs / 3'600 % 24) << ":";
    out << std::setw(2) << (secs / 60 % 60) << ":";
    out << std::setw(2) << (secs % 60) << ".";
    out << std::setw(9) << subsecs;

    return out;
}

} // namespace takatori::chrono
