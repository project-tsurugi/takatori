#include "step_property_scanner.h"

namespace takatori::serializer::details {

using namespace std::string_view_literals;

step_property_scanner::step_property_scanner(
        object_scanner const& scanner,
        object_acceptor& acceptor) noexcept
    : scanner_(scanner)
    , acceptor_(acceptor)
{}

void step_property_scanner::operator()(plan::process const& element) {
    acceptor_.property_begin("operators"sv);
    accept(element.operators());
    acceptor_.property_end();
}

void step_property_scanner::operator()(plan::forward const& element) {
    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();

    acceptor_.property_begin("limit"sv);
    if (auto&& v = element.limit()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();
}

void step_property_scanner::operator()(plan::group const& element) {
    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();

    acceptor_.property_begin("group_keys"sv);
    accept_foreach(element.group_keys());
    acceptor_.property_end();

    acceptor_.property_begin("sort_keys"sv);
    accept_foreach(element.sort_keys());
    acceptor_.property_end();

    acceptor_.property_begin("limit"sv);
    if (auto&& v = element.limit()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();

    acceptor_.property_begin("mode"sv);
    accept(element.mode());
    acceptor_.property_end();
}

void step_property_scanner::operator()(plan::aggregate const& element) {
    acceptor_.property_begin("source_columns"sv);
    accept_foreach(element.source_columns());
    acceptor_.property_end();

    acceptor_.property_begin("destination_columns"sv);
    accept_foreach(element.destination_columns());
    acceptor_.property_end();

    acceptor_.property_begin("group_keys"sv);
    accept_foreach(element.group_keys());
    acceptor_.property_end();

    acceptor_.property_begin("aggregations"sv);
    accept_foreach(element.aggregations());
    acceptor_.property_end();

    acceptor_.property_begin("mode"sv);
    accept(element.mode());
    acceptor_.property_end();
}

void step_property_scanner::operator()(plan::broadcast const& element) {
    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

void step_property_scanner::operator()(plan::discard const&) {
    // no special properties
}

template<class T>
void step_property_scanner::accept(T const& element) {
    if constexpr (std::is_enum_v<T>) { // NOLINT
        acceptor_.string(to_string_view(element));
    } else { // NOLINT
        scanner_(element, acceptor_);
    }
}

template<class Sequence>
void step_property_scanner::accept_foreach(Sequence&& sequence) {
    acceptor_.array_begin();
    for (auto&& v : sequence) {
        accept(v);
    }
    acceptor_.array_end();
}

void step_property_scanner::accept(relation::details::sort_key_element const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("variable"sv);
    accept(element.variable());
    acceptor_.property_end();

    acceptor_.property_begin("direction"sv);
    accept(element.direction());
    acceptor_.property_end();

    acceptor_.struct_end();
}

void step_property_scanner::accept(relation::details::aggregate_element const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("function"sv);
    accept(element.function());
    acceptor_.property_end();

    acceptor_.property_begin("arguments"sv);
    accept_foreach(element.arguments());
    acceptor_.property_end();

    acceptor_.property_begin("destination"sv);
    accept(element.destination());
    acceptor_.property_end();

    acceptor_.struct_end();
}

} // namespace takatori::serializer::details
