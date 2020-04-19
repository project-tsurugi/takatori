#include "value_property_scanner.h"

#include <string_view>

#include "simple_value_scanner.h"

namespace takatori::serializer::details {

using namespace std::string_view_literals;

value_property_scanner::value_property_scanner(object_acceptor& acceptor) noexcept
    : acceptor_(acceptor)
{}

template<class T>
static void simple_property(T const& element, object_acceptor& acceptor) {
    acceptor.property_begin("value"sv);

    simple_value_scanner s { acceptor };
    s(element);

    acceptor.property_end();
}

void value_property_scanner::operator()(value::unknown const& element) {
    simple_property(element, acceptor_);
}

void value_property_scanner::operator()(value::boolean const& element) {
    simple_property(element, acceptor_);
}

void value_property_scanner::operator()(value::int4 const& element) {
    simple_property(element, acceptor_);
}

void value_property_scanner::operator()(value::int8 const& element) {
    simple_property(element, acceptor_);
}

void value_property_scanner::operator()(value::float4 const& element) {
    simple_property(element, acceptor_);
}

void value_property_scanner::operator()(value::float8 const& element) {
    simple_property(element, acceptor_);
}

void value_property_scanner::operator()(value::decimal const& element) {
    simple_property(element, acceptor_);
}

void value_property_scanner::operator()(value::character const& element) {
    simple_property(element, acceptor_);
}

void value_property_scanner::operator()(value::bit const& element) {
    acceptor_.property_begin("value"sv);
    acceptor_.array_begin();
    auto&& v = element.get();
    for (std::size_t i = 0, n = v.size(); i < n; ++i) {
        if (v.test(n - i - 1)) {
            acceptor_.integer(1);
        } else {
            acceptor_.integer(0);
        }
    }
    acceptor_.array_end();
    acceptor_.property_end();
}

static void process(datetime::date value, object_acceptor& acceptor) {
    acceptor.property_begin("year"sv);
    acceptor.unsigned_integer(value.year());
    acceptor.property_end();

    acceptor.property_begin("month"sv);
    acceptor.unsigned_integer(value.month());
    acceptor.property_end();

    acceptor.property_begin("day"sv);
    acceptor.unsigned_integer(value.day());
    acceptor.property_end();
}

void value_property_scanner::operator()(value::date const& element) {
    process(element.get(), acceptor_);
}


static void process(datetime::time_of_day value, object_acceptor& acceptor) {
    acceptor.property_begin("hour"sv);
    acceptor.unsigned_integer(value.hour());
    acceptor.property_end();

    acceptor.property_begin("minute"sv);
    acceptor.unsigned_integer(value.minute());
    acceptor.property_end();

    acceptor.property_begin("second"sv);
    acceptor.unsigned_integer(value.second());
    acceptor.property_end();

    acceptor.property_begin("subsecond"sv);
    acceptor.unsigned_integer(value.subsecond().count());
    acceptor.property_end();
}

void value_property_scanner::operator()(value::time_of_day const& element) {
    process(element.get(), acceptor_);
}

void value_property_scanner::operator()(value::time_point const& element) {
    auto [d, t] = element.get().date_time();
    process(d, acceptor_);
    process(t, acceptor_);
}

static void process(datetime::date_interval value, object_acceptor& acceptor) {
    acceptor.property_begin("year"sv);
    acceptor.integer(value.year());
    acceptor.property_end();

    acceptor.property_begin("month"sv);
    acceptor.integer(value.month());
    acceptor.property_end();

    acceptor.property_begin("day"sv);
    acceptor.integer(value.day());
    acceptor.property_end();
}

static void process(datetime::time_interval value, object_acceptor& acceptor) {
    acceptor.property_begin("hour"sv);
    acceptor.integer(value.hour());
    acceptor.property_end();

    acceptor.property_begin("minute"sv);
    acceptor.integer(value.minute());
    acceptor.property_end();

    acceptor.property_begin("second"sv);
    acceptor.integer(value.second());
    acceptor.property_end();

    acceptor.property_begin("subsecond"sv);
    acceptor.integer(value.subsecond().count());
    acceptor.property_end();
}

void value_property_scanner::operator()(value::datetime_interval const& element) {
    process(element.get().date(), acceptor_);
    process(element.get().time(), acceptor_);
}

} // namespace takatori::serializer::details
