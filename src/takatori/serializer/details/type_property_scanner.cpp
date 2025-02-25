#include "type_property_scanner.h"

#include <string_view>

namespace takatori::serializer::details {

using namespace std::string_view_literals;

type_property_scanner::type_property_scanner(
        object_scanner const& scanner,
        object_acceptor& acceptor) noexcept :
    scanner_ { scanner },
    acceptor_ { acceptor }
{}

void type_property_scanner::operator()(type::boolean const&) {}

void type_property_scanner::operator()(type::int1 const&) {}

void type_property_scanner::operator()(type::int2 const&) {}

void type_property_scanner::operator()(type::int4 const&) {}

void type_property_scanner::operator()(type::int8 const&) {}

void type_property_scanner::operator()(type::float4 const&) {}

void type_property_scanner::operator()(type::float8 const&) {}

void type_property_scanner::operator()(type::decimal const& element) {
    acceptor_.property_begin("precision"sv);
    if (auto&& v = element.precision()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();

    acceptor_.property_begin("scale"sv);
    if (auto&& v = element.scale()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();
}

void type_property_scanner::operator()(type::character const& element) {
    acceptor_.property_begin("varying"sv);
    acceptor_.boolean(element.varying());
    acceptor_.property_end();

    acceptor_.property_begin("length"sv);
    if (auto&& v = element.length()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();
}

void type_property_scanner::operator()(type::octet const& element) {
    acceptor_.property_begin("varying"sv);
    acceptor_.boolean(element.varying());
    acceptor_.property_end();

    acceptor_.property_begin("length"sv);
    if (auto&& v = element.length()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();
}

void type_property_scanner::operator()(type::bit const& element) {
    acceptor_.property_begin("varying"sv);
    acceptor_.boolean(element.varying());
    acceptor_.property_end();

    acceptor_.property_begin("length"sv);
    if (auto&& v = element.length()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();
}

void type_property_scanner::operator()(type::date const&) {}

void type_property_scanner::operator()(type::time_of_day const& element) {
    acceptor_.property_begin("with_time_zone"sv);
    acceptor_.boolean(element.with_time_zone());
    acceptor_.property_end();
}

void type_property_scanner::operator()(type::time_point const& element) {
    acceptor_.property_begin("with_time_zone"sv);
    acceptor_.boolean(element.with_time_zone());
    acceptor_.property_end();
}

void type_property_scanner::operator()(type::datetime_interval const&) {}

void type_property_scanner::operator()(type::blob const&) {}

void type_property_scanner::operator()(type::clob const&) {}

void type_property_scanner::operator()(type::unknown const&) {}

void type_property_scanner::operator()(type::row_id const& element) {
    acceptor_.property_begin("sequence"sv);
    acceptor_.unsigned_integer(element.sequence());
    acceptor_.property_end();
}

void type_property_scanner::operator()(type::declared const& element) {
    acceptor_.property_begin("binding"sv);
    scanner_(element.binding(), acceptor_);
    acceptor_.property_end();
}

void type_property_scanner::operator()(type::extension const& element) {
    acceptor_.property_begin("extension_id"sv);
    acceptor_.unsigned_integer(element.extension_id());
    acceptor_.property_end();
}

} // namespace takatori::serializer::details
