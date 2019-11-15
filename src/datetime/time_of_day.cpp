#include "takatori/datetime/time_of_day.h"

#include "../util/iomanip_util.h"

namespace takatori::datetime {

using hour_unit = std::chrono::duration<std::uint64_t, std::chrono::hours::period>;
using minute_unit = std::chrono::duration<std::uint64_t, std::chrono::minutes::period>;
using second_unit = std::chrono::duration<std::uint64_t, std::chrono::seconds::period>;

time_of_day::time_of_day(
        std::uint32_t hour,
        std::uint32_t minute,
        std::uint32_t second,
        time_of_day::time_unit subsecond) noexcept
    : elapsed_(
            hour_unit(hour)
            + minute_unit(minute)
            + second_unit(second)
            + subsecond)
{}

std::uint32_t time_of_day::hour() const noexcept {
    return static_cast<std::uint32_t>(std::chrono::floor<hour_unit>(elapsed_).count());
}

std::uint32_t time_of_day::minute() const noexcept {
    return static_cast<std::uint32_t>(std::chrono::floor<minute_unit>(elapsed_).count() % 60);
}

std::uint32_t time_of_day::second() const noexcept {
    return static_cast<std::uint32_t>(std::chrono::floor<second_unit>(elapsed_).count() % 60);
}

time_of_day::subsecond_unit time_of_day::subsecond() const noexcept {
    return subsecond_unit { static_cast<subsecond_unit::rep>(elapsed_.count() % std::nano::den) };
}

std::ostream& operator<<(std::ostream& out, time_of_day const& value) {
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
