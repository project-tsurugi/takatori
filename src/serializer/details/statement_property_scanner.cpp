#include "statement_property_scanner.h"

namespace takatori::serializer::details {

using namespace std::string_view_literals;

statement_property_scanner::statement_property_scanner(
        object_scanner const& scanner,
        object_acceptor& acceptor) noexcept
    : scanner_(scanner)
    , acceptor_(acceptor)
{}

void statement_property_scanner::operator()(statement::execute const& element) {
    acceptor_.property_begin("execution_plan"sv);
    accept(element.execution_plan());
    acceptor_.property_end();
}

void statement_property_scanner::operator()(statement::write const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("destination"sv);
    accept(element.destination());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();

    acceptor_.property_begin("tuples"sv);
    accept_foreach(element.tuples());
    acceptor_.property_end();
}

void statement_property_scanner::operator()(statement::extension const& element) {
    acceptor_.property_begin("extension_id"sv);
    acceptor_.unsigned_integer(element.extension_id());
    acceptor_.property_end();
}

template<class T>
void statement_property_scanner::accept(util::optional_ptr<T const> element) {
    if (element) {
        scanner_(*element, acceptor_);
    }
}

template<class T>
void statement_property_scanner::accept(T const& element) {
    if constexpr (std::is_enum_v<T>) { // NOLINT
        acceptor_.string(to_string_view(element));
    } else { // NOLINT
        scanner_(element, acceptor_);
    }
}

template<class Sequence>
void statement_property_scanner::accept_foreach(Sequence&& sequence) {
    acceptor_.array_begin();
    for (auto&& v : sequence) {
        accept(v);
    }
    acceptor_.array_end();
}

void statement_property_scanner::accept(statement::details::write_tuple const& element) {
    accept_foreach(element.elements());
}

} // namespace takatori::serializer::details
