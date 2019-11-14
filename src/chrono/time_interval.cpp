#include "takatori/chrono/time_interval.h"

#include "../util/iomanip_util.h"

namespace takatori::chrono {

template<class T>
static char sign(T value) noexcept {
    return value >= 0 ? '+' : '-';
}

std::ostream& operator<<(std::ostream& out, time_interval const& value) {
    auto months = std::abs(value.total_months_);
    auto secs = std::abs(value.total_nanoseconds_ / 1'000'000'000);
    auto subsecs = std::abs(value.total_nanoseconds_ % 1'000'000'000);

    util::instant_fill fill { out, '0' };
    out << "time_interval(";
    out << "[" << sign(value.total_months_) << "]";
    out << (months / 12) << "-";
    out << (months % 12) << "-";
    out << "[" << sign(value.total_nanoseconds_) << "]";
    out << (secs / 86'400) << "T";
    out << std::setw(2) << (secs / 3'600 % 24) << ":";
    out << std::setw(2) << (secs / 60 % 60) << ":";
    out << std::setw(2) << (secs % 60) << ".";
    out << std::setw(9) << subsecs;
    out << ")";

    return out;
}

} // namespace takatori::chrono
