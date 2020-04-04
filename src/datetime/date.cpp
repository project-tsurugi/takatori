#include <takatori/datetime/date.h>

#include "date_util.h"
#include "printing.h"

namespace takatori::datetime {

date::date(size_type year, size_type month, size_type day) noexcept
    : elapsed_(util::to_days_since_epoch(year, month, day))
{}

date::size_type date::year() const noexcept {
    auto v = util::to_year_and_day(elapsed_);
    return v.year;
}

date::size_type date::month() const noexcept {
    auto yad = util::to_year_and_day(elapsed_);
    auto v = util::to_month_and_day(yad.year, yad.day_of_year);
    return v.month;
}

date::size_type date::day() const noexcept {
    auto yad = util::to_year_and_day(elapsed_);
    auto v = util::to_month_and_day(yad.year, yad.day_of_year);
    return v.day_of_month;
}

std::ostream& operator<<(std::ostream& out, date value) {
    out << "date(";
    print(out, value);
    out << ")";
    return out;
}

} // namespace takatori::datetime
