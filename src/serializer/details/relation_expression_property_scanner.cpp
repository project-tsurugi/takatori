#include "relation_expression_property_scanner.h"

namespace takatori::serializer::details {

using namespace std::string_view_literals;

relation_expression_property_scanner::relation_expression_property_scanner(
        object_scanner& scanner,
        object_acceptor& acceptor) noexcept
    : scanner_(scanner)
    , acceptor_(acceptor)
{}

void relation_expression_property_scanner::operator()(relation::find const& element) {
    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();

    acceptor_.property_begin("keys"sv);
    accept_foreach(element.keys());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::scan const& element) {
    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();

    acceptor_.property_begin("lower"sv);
    accept(element.lower());
    acceptor_.property_end();

    acceptor_.property_begin("upper"sv);
    accept(element.upper());
    acceptor_.property_end();

    acceptor_.property_begin("limit"sv);
    if (auto&& v = element.limit()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::join_find const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();

    acceptor_.property_begin("keys"sv);
    accept_foreach(element.keys());
    acceptor_.property_end();

    acceptor_.property_begin("condition"sv);
    accept(element.condition());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::join_scan const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();

    acceptor_.property_begin("lower"sv);
    accept(element.lower());
    acceptor_.property_end();

    acceptor_.property_begin("upper"sv);
    accept(element.upper());
    acceptor_.property_end();

    acceptor_.property_begin("condition"sv);
    accept(element.condition());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::project const& element) {
    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::filter const& element) {
    acceptor_.property_begin("condition"sv);
    accept(element.optional_condition());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::buffer const&) {
    // no special properties
}

void relation_expression_property_scanner::operator()(relation::emit const& element) {
    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::write const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("destination"sv);
    accept(element.destination());
    acceptor_.property_end();

    acceptor_.property_begin("keys"sv);
    accept_foreach(element.keys());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::intermediate::join const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("key_pairs"sv);
    accept_foreach(element.key_pairs());
    acceptor_.property_end();

    acceptor_.property_begin("lower"sv);
    accept(element.lower());
    acceptor_.property_end();

    acceptor_.property_begin("upper"sv);
    accept(element.upper());
    acceptor_.property_end();

    acceptor_.property_begin("condition"sv);
    accept(element.condition());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::intermediate::aggregate const& element) {
    acceptor_.property_begin("group_keys"sv);
    accept_foreach(element.group_keys());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::intermediate::distinct const& element) {
    acceptor_.property_begin("group_keys"sv);
    accept_foreach(element.group_keys());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::intermediate::limit const& element) {
    acceptor_.property_begin("count"sv);
    if (auto&& v = element.count()) {
        acceptor_.unsigned_integer(*v);
    }
    acceptor_.property_end();

    acceptor_.property_begin("group_keys"sv);
    accept_foreach(element.group_keys());
    acceptor_.property_end();

    acceptor_.property_begin("sort_keys"sv);
    accept_foreach(element.sort_keys());
    acceptor_.property_end();

}

void relation_expression_property_scanner::operator()(relation::intermediate::union_ const& element) {
    acceptor_.property_begin("quantifier"sv);
    accept(element.quantifier());
    acceptor_.property_end();

    acceptor_.property_begin("mappings"sv);
    accept_foreach(element.mappings());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::intermediate::intersection const& element) {
    acceptor_.property_begin("quantifier"sv);
    accept(element.quantifier());
    acceptor_.property_end();

    acceptor_.property_begin("group_key_pairs"sv);
    accept_foreach(element.group_key_pairs());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::intermediate::difference const& element) {
    acceptor_.property_begin("quantifier"sv);
    accept(element.quantifier());
    acceptor_.property_end();

    acceptor_.property_begin("group_key_pairs"sv);
    accept_foreach(element.group_key_pairs());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::intermediate::escape const& element) {
    acceptor_.property_begin("mappings"sv);
    accept_foreach(element.mappings());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::step::join const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("condition"sv);
    accept(element.condition());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::step::aggregate const& element) {
    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::step::intersection const&) {
    // no special properties
}

void relation_expression_property_scanner::operator()(relation::step::difference const&) {
    // no special properties
}

void relation_expression_property_scanner::operator()(relation::step::flatten const&) {
    // no special properties
}

void relation_expression_property_scanner::operator()(relation::step::take_flat const& element) {
    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::step::take_group const& element) {
    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::step::take_cogroup const& element) {
    acceptor_.property_begin("groups"sv);
    accept_foreach(element.groups());
    acceptor_.property_end();
}

void relation_expression_property_scanner::operator()(relation::step::offer const& element) {
    acceptor_.property_begin("destination"sv);
    accept(element.destination());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();
}

template<class T>
void relation_expression_property_scanner::accept(util::optional_ptr<T const> element) {
    if (element) {
        scanner_(*element, acceptor_);
    }
}

template<class T>
void relation_expression_property_scanner::accept(T const& element) {
    if constexpr (std::is_enum_v<T>) { // NOLINT
        acceptor_.string(to_string_view(element));
    } else { // NOLINT
        scanner_(element, acceptor_);
    }
}

template<class Sequence>
void relation_expression_property_scanner::accept_foreach(Sequence&& sequence) {
    acceptor_.array_begin();
    for (auto&& v : sequence) {
        accept(v);
    }
    acceptor_.array_end();
}

void relation_expression_property_scanner::accept(relation::details::mapping_element const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("destination"sv);
    accept(element.destination());
    acceptor_.property_end();

    acceptor_.struct_end();
}

template<class T>
void relation_expression_property_scanner::accept(relation::details::search_key_element<T> const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("variable"sv);
    accept(element.variable());
    acceptor_.property_end();

    acceptor_.property_begin("value"sv);
    accept(element.optional_value());
    acceptor_.property_end();

    acceptor_.struct_end();
}

template<class T, class U>
void relation_expression_property_scanner::accept(relation::details::range_endpoint<T, U> const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("keys"sv);
    accept_foreach(element.keys());
    acceptor_.property_end();

    acceptor_.property_begin("kind"sv);
    accept(element.kind());
    acceptor_.property_end();

    acceptor_.struct_end();
}

template<class T>
void relation_expression_property_scanner::accept(scalar::details::variable_declarator<T> const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("variable"sv);
    accept(element.variable());
    acceptor_.property_end();

    acceptor_.property_begin("value"sv);
    accept(element.optional_value());
    acceptor_.property_end();

    acceptor_.struct_end();
}

void relation_expression_property_scanner::accept(relation::details::key_pair_element const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("left"sv);
    accept(element.left());
    acceptor_.property_end();

    acceptor_.property_begin("right"sv);
    accept(element.right());
    acceptor_.property_end();

    acceptor_.struct_end();
}

void relation_expression_property_scanner::accept(relation::details::sort_key_element const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("variable"sv);
    accept(element.variable());
    acceptor_.property_end();

    acceptor_.property_begin("direction"sv);
    accept(element.direction());
    acceptor_.property_end();

    acceptor_.struct_end();
}

void relation_expression_property_scanner::accept(relation::details::emit_element const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("name"sv);
    if (auto&& v = element.name()) {
        acceptor_.string(*v);
    }
    acceptor_.property_end();

    acceptor_.struct_end();
}

void relation_expression_property_scanner::accept(relation::details::aggregate_element const& element) {
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

void relation_expression_property_scanner::accept(relation::details::union_element const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("left"sv);
    if (auto&& v = element.left()) {
        accept(*v);
    }
    acceptor_.property_end();

    acceptor_.property_begin("right"sv);
    if (auto&& v = element.right()) {
        accept(*v);
    }
    acceptor_.property_end();

    acceptor_.property_begin("destination"sv);
    accept(element.destination());
    acceptor_.property_end();

    acceptor_.struct_end();
}

void relation_expression_property_scanner::accept(relation::details::cogroup_element const& element) {
    acceptor_.struct_begin();

    acceptor_.property_begin("source"sv);
    accept(element.source());
    acceptor_.property_end();

    acceptor_.property_begin("columns"sv);
    accept_foreach(element.columns());
    acceptor_.property_end();

    acceptor_.struct_end();
}

} // namespace takatori::serializer::details
