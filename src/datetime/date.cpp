#include "takatori/datetime/date.h"

#include "date_util.h"

namespace takatori::datetime {

namespace {

} // namespace

date::date(std::uint32_t year, std::uint32_t month, std::uint32_t day) noexcept
    : elapsed_(util::to_days_since_epoch(year, month, day))
{}

std::uint32_t date::year() const noexcept {
    auto v = util::to_year_and_day(elapsed_);
    return v.year;
}

std::uint32_t date::month() const noexcept {
    auto yad = util::to_year_and_day(elapsed_);
    auto v = util::to_month_and_day(yad.year, yad.day_of_year);
    return v.month;
}

std::uint32_t date::day() const noexcept {
    auto yad = util::to_year_and_day(elapsed_);
    auto v = util::to_month_and_day(yad.year, yad.day_of_year);
    return v.day_of_month;
}

std::ostream& operator<<(std::ostream& out, date const& value) {
    out << "date(";
    value.print_body(out);
    out << ")";
    return out;
}

std::ostream& date::print_body(std::ostream& out) const {
    return out << year() << "-"
               << month() << "-"
               << day();
}

} // namespace takatori::datetime
