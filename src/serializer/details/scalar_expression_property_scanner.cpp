#include "scalar_expression_property_scanner.h"

namespace takatori::serializer::details {

using namespace std::string_view_literals;

scalar_expression_property_scanner::scalar_expression_property_scanner(
        object_scanner const& scanner,
        object_acceptor& acceptor) noexcept
    : scanner_(scanner)
    , acceptor_(acceptor)
{}

void scalar_expression_property_scanner::operator()(scalar::immediate const& element) {
    acceptor_.property_begin("value"sv);
    accept(element.optional_value());
    acceptor_.property_end();

    acceptor_.property_begin("type"sv);
    accept(element.optional_type());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::variable_reference const& element) {
    acceptor_.property_begin("variable"sv);
    accept(element.variable());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::unary const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("operand"sv);
    accept(element.optional_operand());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::cast const& element) {
    acceptor_.property_begin("type"sv);
    accept(element.optional_type());
    acceptor_.property_end();

    acceptor_.property_begin("loss_policy"sv);
    accept(element.loss_policy());
    acceptor_.property_end();

    acceptor_.property_begin("operand"sv);
    accept(element.optional_operand());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::binary const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("left"sv);
    accept(element.optional_left());
    acceptor_.property_end();

    acceptor_.property_begin("right"sv);
    accept(element.optional_right());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::compare const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("left"sv);
    accept(element.optional_left());
    acceptor_.property_end();

    acceptor_.property_begin("right"sv);
    accept(element.optional_right());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::match const& element) {
    acceptor_.property_begin("operator_kind"sv);
    accept(element.operator_kind());
    acceptor_.property_end();

    acceptor_.property_begin("input"sv);
    accept(element.optional_input());
    acceptor_.property_end();

    acceptor_.property_begin("pattern"sv);
    accept(element.optional_pattern());
    acceptor_.property_end();

    acceptor_.property_begin("escape"sv);
    accept(element.optional_escape());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::conditional const& element) {
    acceptor_.property_begin("alternatives"sv);
    acceptor_.array_begin();
    for (auto&& alternative : element.alternatives()) {
        acceptor_.struct_begin();

        acceptor_.property_begin("condition"sv);
        accept(alternative.optional_condition());
        acceptor_.property_end();

        acceptor_.property_begin("body"sv);
        accept(alternative.optional_body());
        acceptor_.property_end();

        acceptor_.struct_end();
    }
    acceptor_.array_end();
    acceptor_.property_end();

    acceptor_.property_begin("default_expression"sv);
    accept(element.default_expression());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::coalesce const& element) {
    acceptor_.property_begin("alternatives"sv);
    acceptor_.array_begin();
    for (auto&& alternative : element.alternatives()) {
        accept(alternative);
    }
    acceptor_.array_end();
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::let const& element) {
    acceptor_.property_begin("variables"sv);
    acceptor_.array_begin();
    for (auto&& decl : element.variables()) {
        acceptor_.struct_begin();

        acceptor_.property_begin("variable"sv);
        accept(decl.variable());
        acceptor_.property_end();

        acceptor_.property_begin("value"sv);
        accept(decl.optional_value());
        acceptor_.property_end();

        acceptor_.struct_end();
    }
    acceptor_.array_end();
    acceptor_.property_end();

    acceptor_.property_begin("body"sv);
    accept(element.optional_body());
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::function_call const& element) {
    acceptor_.property_begin("function"sv);
    accept(element.function());
    acceptor_.property_end();

    acceptor_.property_begin("arguments"sv);
    acceptor_.array_begin();
    for (auto&& argument : element.arguments()) {
        accept(argument);
    }
    acceptor_.array_end();
    acceptor_.property_end();
}

void scalar_expression_property_scanner::operator()(scalar::extension const& element) {
    acceptor_.property_begin("extension_id"sv);
    acceptor_.unsigned_integer(element.extension_id());
    acceptor_.property_end();
}

template<class T>
void scalar_expression_property_scanner::accept(util::optional_ptr<T const> element) {
    if (element) {
        scanner_(*element, acceptor_);
    }
}

template<class T>
void scalar_expression_property_scanner::accept(T const& element) {
    if constexpr (std::is_enum_v<T>) { // NOLINT
        acceptor_.string(to_string_view(element));
    } else { // NOLINT
        scanner_(element, acceptor_);
    }
}

} // namespace takatori::serializer::details
