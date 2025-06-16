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

void simple_value_scanner::operator()(value::octet const& element) {
    thread_local std::string buffer {}; // NOLINT(misc-use-internal-linkage)
    static constexpr std::array<char, 16> hex {
            '0', '1', '2', '3',
            '4', '5', '6', '7',
            '8', '9', 'a', 'b',
            'c', 'd', 'e', 'f',
    };
    auto v = element.get();
    buffer.resize(v.size() * 2, 0);
    for (std::size_t i = 0, n = v.size(); i < n; ++i) {
        std::size_t c = static_cast<unsigned char>(v[i]);
        auto hi = static_cast<std::size_t>((c >> 4U) & 0x0fU);
        auto lo = static_cast<std::size_t>(c & 0x0fU);
        buffer[i * 2 + 0] = hex[hi]; // NOLINT
        buffer[i * 2 + 1] = hex[lo]; // NOLINT
    }
    acceptor_.string(buffer);
    buffer.clear();
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

void simple_value_scanner::operator()(value::extension const&) {
    acceptor_.string("(extension)");
}

} // namespace takatori::serializer::details
