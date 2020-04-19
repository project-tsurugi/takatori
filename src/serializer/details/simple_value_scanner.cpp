#include "simple_value_scanner.h"

namespace takatori::serializer::details {

simple_value_scanner::simple_value_scanner(object_acceptor& acceptor) noexcept
    : acceptor_(acceptor)
{}

void simple_value_scanner::operator()(value::unknown const& element) {
    acceptor_.value(element.get());
}

void simple_value_scanner::operator()(value::boolean const& element) {
    acceptor_.boolean(element.get());
}

void simple_value_scanner::operator()(value::int4 const& element) {
    acceptor_.integer(element.get());
}

void simple_value_scanner::operator()(value::int8 const& element) {
    acceptor_.integer(element.get());
}

void simple_value_scanner::operator()(value::float4 const& element) {
    acceptor_.binary_float(element.get());
}

void simple_value_scanner::operator()(value::float8 const& element) {
    acceptor_.binary_float(element.get());
}

void simple_value_scanner::operator()(value::decimal const& element) {
    acceptor_.number(element.get());
}

void simple_value_scanner::operator()(value::character const& element) {
    acceptor_.string(element.get());
}

void simple_value_scanner::operator()(value::bit const& element) {
    acceptor_.value(element.get());
}

void simple_value_scanner::operator()(value::date const& element) {
    acceptor_.value(element.get());
}

void simple_value_scanner::operator()(value::time_of_day const& element) {
    acceptor_.value(element.get());
}

void simple_value_scanner::operator()(value::time_point const& element) {
    acceptor_.value(element.get());
}

void simple_value_scanner::operator()(value::datetime_interval const& element) {
    acceptor_.value(element.get());
}

} // namespace takatori::serializer::details
