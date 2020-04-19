#include <takatori/serializer/object_scanner.h>

#include <takatori/value/dispatch.h>

#include <takatori/type/dispatch.h>

#include <takatori/scalar/dispatch.h>

#include <takatori/relation/graph.h>
#include <takatori/relation/intermediate/dispatch.h>
#include <takatori/relation/step/dispatch.h>

#include <takatori/plan/graph.h>
#include <takatori/plan/dispatch.h>

#include <takatori/util/detect.h>

#include "details/simple_value_scanner.h"
#include "details/value_property_scanner.h"
#include "details/type_property_scanner.h"
#include "details/scalar_expression_property_scanner.h"
#include "details/relation_expression_property_scanner.h"
#include "details/step_property_scanner.h"

namespace takatori::serializer {

using namespace std::string_view_literals;

template<class T>
using tag_t = decltype(T::tag);

template<class T>
constexpr bool has_tag_v = util::is_detected_v<tag_t, T>;

void object_scanner::operator()(descriptor::variable const& element, object_acceptor& acceptor) {
    process_default(element, acceptor);
}

void object_scanner::operator()(descriptor::relation const& element, object_acceptor& acceptor) {
    process_default(element, acceptor);
}

void object_scanner::operator()(descriptor::function const& element, object_acceptor& acceptor) {
    process_default(element, acceptor);
}

void object_scanner::operator()(descriptor::aggregate_function const& element, object_acceptor& acceptor) {
    process_default(element, acceptor);
}

void object_scanner::operator()(descriptor::declared_type const& element, object_acceptor& acceptor) {
    process_default(element, acceptor);
}

void object_scanner::operator()(value::data const& element, object_acceptor& acceptor) {
    if (verbose()) {
        process_envelope(element, acceptor);
    } else {
        details::simple_value_scanner s { acceptor };
        value::dispatch(s, element);
    }
}

void object_scanner::operator()(type::data const& element, object_acceptor& acceptor) {
    process_default(element, acceptor);
}

void object_scanner::operator()(scalar::expression const& element, object_acceptor& acceptor) {
    process_envelope(element, acceptor);
}

template<class T>
static void accept_ref(T const& element, object_acceptor& acceptor) {
    acceptor.property_begin("kind"sv);
    acceptor.string(to_string_view(element.kind()));
    acceptor.property_end();

    acceptor.property_begin("this"sv);
    acceptor.pointer(std::addressof(element));
    acceptor.property_end();
}

template<class T, graph::port_direction D>
static void accept_port(graph::port<T, D> const& port, object_acceptor& acceptor) {
    acceptor.struct_begin();
    acceptor.property_begin("index"sv);
    acceptor.unsigned_integer(port.index());
    acceptor.property_end();

    acceptor.property_begin("opposite"sv);
    if (auto&& opposite = port.opposite()) {
        acceptor.struct_begin();

        accept_ref(opposite->owner(), acceptor);

        acceptor.property_begin("index"sv);
        acceptor.unsigned_integer(opposite->index());
        acceptor.property_end();

        acceptor.struct_end();
    }
    acceptor.property_end();
    acceptor.struct_end();
}

void object_scanner::operator()(relation::expression const& element, object_acceptor& acceptor) {
    acceptor.struct_begin();

    accept_ref(element, acceptor);

    properties(element, acceptor);

    acceptor.property_begin("input_ports"sv);
    acceptor.array_begin();
    for (auto&& p : element.input_ports()) {
        accept_port(p, acceptor);
    }
    acceptor.array_end();
    acceptor.property_end();

    acceptor.property_begin("output_ports"sv);
    acceptor.array_begin();
    for (auto&& p : element.output_ports()) {
        accept_port(p, acceptor);
    }
    acceptor.array_end();
    acceptor.property_end();

    acceptor.struct_end();
}

void object_scanner::operator()(relation::expression::graph_type const& element, object_acceptor& acceptor) {
    acceptor.array_begin();
    relation::sort_from_upstream(element, [&](relation::expression const& v) {
        (*this)(v, acceptor);
    });
    acceptor.array_end();
}

void object_scanner::operator()(plan::step const& element, object_acceptor& acceptor) {
    acceptor.struct_begin();

    accept_ref(element, acceptor);

    properties(element, acceptor);

    acceptor.property_begin("upstreams"sv);
    acceptor.array_begin();
    plan::upstream_enumerator {}(element, [&](plan::step const& s) {
        acceptor.struct_begin();
        accept_ref(s, acceptor);
        acceptor.struct_end();
    });
    acceptor.array_end();
    acceptor.property_end();

    acceptor.property_begin("downstreams"sv);
    acceptor.array_begin();
    plan::downstream_enumerator {}(element, [&](plan::step const& s) {
        acceptor.struct_begin();
        accept_ref(s, acceptor);
        acceptor.struct_end();
    });
    acceptor.array_end();
    acceptor.property_end();

    acceptor.struct_end();
}

void object_scanner::operator()(plan::step::graph_type const& element, object_acceptor& acceptor) {
    acceptor.array_begin();
    plan::sort_from_upstream(element, [&](plan::step const& v) {
        (*this)(v, acceptor);
    });
    acceptor.array_end();
}

template<descriptor::descriptor_kind Kind>
static void process_descriptor_body(descriptor::element<Kind> const& element, object_acceptor& acceptor) {
    acceptor.property_begin("entity"sv);
    acceptor.value(element.optional_entity());
    acceptor.property_end();
}

void object_scanner::properties(descriptor::variable const& element, object_acceptor& acceptor) {
    process_descriptor_body(element, acceptor);
}

void object_scanner::properties(descriptor::relation const& element, object_acceptor& acceptor) {
    process_descriptor_body(element, acceptor);
}

void object_scanner::properties(descriptor::function const& element, object_acceptor& acceptor) {
    process_descriptor_body(element, acceptor);
}

void object_scanner::properties(descriptor::aggregate_function const& element, object_acceptor& acceptor) {
    process_descriptor_body(element, acceptor);
}

void object_scanner::properties(descriptor::declared_type const& element, object_acceptor& acceptor) {
    process_descriptor_body(element, acceptor);
}

void object_scanner::properties(value::data const& element, object_acceptor& acceptor) {
    details::value_property_scanner s { acceptor };
    value::dispatch(s, element);
}

void object_scanner::properties(type::data const& element, object_acceptor& acceptor) {
    details::type_property_scanner s { *this, acceptor };
    type::dispatch(s, element);
}

void object_scanner::properties(scalar::expression const& element, object_acceptor& acceptor) {
    details::scalar_expression_property_scanner s { *this, acceptor };
    scalar::dispatch(s, element);
}

void object_scanner::properties(relation::expression const& element, object_acceptor& acceptor) {
    details::relation_expression_property_scanner s { *this, acceptor };
    if (relation::is_available_in_intermediate_plan(element.kind())) {
        relation::intermediate::dispatch(s, element);
    } else {
        relation::step::dispatch(s, element);
    }
}

void object_scanner::properties(plan::step const& element, object_acceptor& acceptor) {
    details::step_property_scanner s { *this, acceptor };
    plan::dispatch(s, element);
}

template<class T>
void object_scanner::process_default(T const& element, object_acceptor& acceptor) {
    if (!verbose()) {
        acceptor.value(element);
    } else {
        process_envelope(element, acceptor);
    }
}

template<class T>
void object_scanner::process_envelope(T const& element, object_acceptor& acceptor) {
    acceptor.struct_begin();

    acceptor.property_begin("kind"sv);
    if constexpr (has_tag_v<T>) { // NOLINT
        acceptor.string(to_string_view(T::tag));
    } else { // NOLINT
        acceptor.string(to_string_view(element.kind()));
    }
    acceptor.property_end();

    properties(element, acceptor);

    acceptor.struct_end();
}

} // namespace takatori::serializer
