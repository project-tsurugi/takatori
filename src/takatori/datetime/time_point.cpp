#include <takatori/datetime/time_point.h>

#include "printing.h"

namespace takatori::datetime {

using date_unit = std::chrono::duration<std::uint64_t, std::ratio<86'400>>;

time_point::time_point(datetime::date date, datetime::time_of_day time) noexcept
    : elapsed_(date_unit(date.days_since_epoch()) + time.time_since_epoch())
{}

time_point time_point::now() {
    return time_point { std::chrono::system_clock::now() };
}

std::ostream& operator<<(std::ostream& out, time_point value) {
    out << "time_point(";
    print(out, value.date());
    out << " ";
    print(out, value.time());
    out << ")";
    return out;
}

time_point::time_unit
time_point::from_chrono(std::chrono::time_point<std::chrono::system_clock, clock_unit> time) {
    // unix time epoch ~ our epoch
    constexpr clock_unit unix_time_since_epoch = date_unit(25'567);
    return std::chrono::duration_cast<time_unit>(time.time_since_epoch() + unix_time_since_epoch);
}

} // namespace takatori::datetime
