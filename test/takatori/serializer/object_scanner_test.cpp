#include <takatori/serializer/object_scanner.h>

#include <string>

#include <gtest/gtest.h>

#include <takatori/serializer/json_printer.h>

#include <takatori/value/primitive.h>
#include <takatori/value/decimal.h>
#include <takatori/value/character.h>
#include <takatori/value/octet.h>
#include <takatori/value/bit.h>
#include <takatori/value/date.h>
#include <takatori/value/time_of_day.h>
#include <takatori/value/time_point.h>
#include <takatori/value/datetime_interval.h>

#include <takatori/type/primitive.h>
#include <takatori/type/decimal.h>
#include <takatori/type/character.h>
#include <takatori/type/octet.h>
#include <takatori/type/bit.h>
#include <takatori/type/date.h>
#include <takatori/type/time_of_day.h>
#include <takatori/type/time_point.h>
#include <takatori/type/datetime_interval.h>
#include <takatori/type/lob.h>
#include <takatori/type/row_id.h>
#include <takatori/type/declared.h>
#include <takatori/type/extension.h>

#include <takatori/scalar/immediate.h>
#include <takatori/scalar/variable_reference.h>
#include <takatori/scalar/unary.h>
#include <takatori/scalar/cast.h>
#include <takatori/scalar/binary.h>
#include <takatori/scalar/compare.h>
#include <takatori/scalar/match.h>
#include <takatori/scalar/conditional.h>
#include <takatori/scalar/coalesce.h>
#include <takatori/scalar/let.h>
#include <takatori/scalar/function_call.h>
#include <takatori/scalar/extension.h>

#include <takatori/relation/find.h>
#include <takatori/relation/scan.h>
#include <takatori/relation/join_find.h>
#include <takatori/relation/join_scan.h>
#include <takatori/relation/project.h>
#include <takatori/relation/filter.h>
#include <takatori/relation/buffer.h>
#include <takatori/relation/identify.h>
#include <takatori/relation/emit.h>
#include <takatori/relation/write.h>
#include <takatori/relation/values.h>

#include <takatori/relation/intermediate/join.h>
#include <takatori/relation/intermediate/aggregate.h>
#include <takatori/relation/intermediate/distinct.h>
#include <takatori/relation/intermediate/limit.h>
#include <takatori/relation/intermediate/union.h>
#include <takatori/relation/intermediate/intersection.h>
#include <takatori/relation/intermediate/difference.h>
#include <takatori/relation/intermediate/escape.h>

#include <takatori/relation/step/join.h>
#include <takatori/relation/step/aggregate.h>
#include <takatori/relation/step/intersection.h>
#include <takatori/relation/step/difference.h>
#include <takatori/relation/step/flatten.h>
#include <takatori/relation/step/take_flat.h>
#include <takatori/relation/step/take_group.h>
#include <takatori/relation/step/take_cogroup.h>
#include <takatori/relation/step/offer.h>

#include <takatori/plan/process.h>
#include <takatori/plan/forward.h>
#include <takatori/plan/group.h>
#include <takatori/plan/aggregate.h>
#include <takatori/plan/broadcast.h>
#include <takatori/plan/discard.h>
#include <takatori/plan/graph.h>

#include <takatori/statement/execute.h>
#include <takatori/statement/write.h>
#include <takatori/statement/create_table.h>
#include <takatori/statement/drop_table.h>
#include <takatori/statement/create_index.h>
#include <takatori/statement/drop_index.h>
#include <takatori/statement/empty.h>
#include <takatori/statement/extension.h>

#include <takatori/testing/descriptors.h>
#include <takatori/type/dummy_extension.h>
#include <takatori/value/dummy_extension.h>
#include <takatori/scalar/dummy_extension.h>
#include <takatori/statement/dummy_extension.h>

namespace takatori::serializer {

using namespace ::takatori::testing;

class object_scanner_test : public ::testing::Test {
public:
    bool verbose { true };

    template<class T>
    void print(T const& element) {
        std::cout << ::testing::UnitTest::GetInstance()->current_test_info()->name() << ": ";

        json_printer printer { std::cout };
        object_scanner scanner { verbose };
        scanner(element, printer);

        std::cout << std::endl;
        if (printer.depth() != 0) {
            throw std::domain_error("invalid JSON depth");
        }
    }
};

static scalar::immediate const_int4() {
    return scalar::immediate {
            value::int4 { 100 },
            type::int4 {},
    };
}

static scalar::immediate const_boolean() {
    return scalar::immediate {
            value::boolean { true },
            type::boolean {},
    };
}

// NOTE: this only checks if it passes without exceptions

// name

TEST_F(object_scanner_test, name_identifier) {
    print(name::identifier { "hello" });
}

TEST_F(object_scanner_test, name_name) {
    print(name::name { "a", "b", "c" });
}

// descriptor

TEST_F(object_scanner_test, descriptor_variable) {
    print(vardesc(1));
}

TEST_F(object_scanner_test, descriptor_relation) {
    print(tabledesc("T"));
}

TEST_F(object_scanner_test, descriptor_function) {
    print(funcdesc(2));
}

TEST_F(object_scanner_test, descriptor_aggregate_function) {
    print(aggdesc("COUNT"));
}

TEST_F(object_scanner_test, descriptor_declared_type) {
    print(typedesc("point"));
}

// value

TEST_F(object_scanner_test, value_boolean) {
    print(value::boolean { true });
}

TEST_F(object_scanner_test, value_int4) {
    print(value::int4 { 100 });
}

TEST_F(object_scanner_test, value_int8) {
    print(value::int8 { 100 });
}

TEST_F(object_scanner_test, value_float4) {
    print(value::float4 { 1.25 });
}

TEST_F(object_scanner_test, value_float8) {
    print(value::float8 { -1.0 });
}

TEST_F(object_scanner_test, value_decimal) {
    print(value::decimal { "3.14" });
}

TEST_F(object_scanner_test, value_character) {
    print(value::character { "\"Hello, world!\"" });
}

TEST_F(object_scanner_test, value_octet) {
    print(value::octet { "Hello" });
}

TEST_F(object_scanner_test, value_octet_simple) {
    verbose = false;
    print(value::octet { "Hello" });
}

TEST_F(object_scanner_test, value_bit) {
    print(value::bit { "00001111" });
}

TEST_F(object_scanner_test, value_date) {
    print(value::date { 2020, 4, 7 });
}

TEST_F(object_scanner_test, value_time_of_day) {
    print(value::time_of_day { 12, 34, 56 });
}

TEST_F(object_scanner_test, value_time_point) {
    print(value::time_point { 2020, 1, 2, 3, 4, 5 });
}

TEST_F(object_scanner_test, value_datetime_interval) {
    print(value::datetime_interval { +1, -2, +3 });
}

TEST_F(object_scanner_test, value_unknown) {
    print(value::unknown { value::unknown_kind::not_a_number });
}

TEST_F(object_scanner_test, value_extension) {
    print(value::dummy_extension { "Hello, world!" });
}

// type

TEST_F(object_scanner_test, type_boolean) {
    print(type::boolean {});
}

TEST_F(object_scanner_test, type_int4) {
    print(type::int4 {});
}

TEST_F(object_scanner_test, type_int8) {
    print(type::int8 {});
}

TEST_F(object_scanner_test, type_float4) {
    print(type::float4 {});
}

TEST_F(object_scanner_test, type_float8) {
    print(type::float8 {});
}

TEST_F(object_scanner_test, type_decimal) {
    print(type::decimal { 18, 8 });
}

TEST_F(object_scanner_test, type_character) {
    print(type::character { type::varying, 16 });
}

TEST_F(object_scanner_test, type_octet) {
    print(type::octet { 64 });
}

TEST_F(object_scanner_test, type_bit) {
    print(type::bit { 64 });
}

TEST_F(object_scanner_test, type_date) {
    print(type::date {});
}

TEST_F(object_scanner_test, type_time_of_day) {
    print(type::time_of_day {});
}

TEST_F(object_scanner_test, type_time_point) {
    print(type::time_point { type::with_time_zone });
}

TEST_F(object_scanner_test, type_datetime_interval) {
    print(type::datetime_interval {});
}

TEST_F(object_scanner_test, type_blob) {
    print(type::blob {});
}

TEST_F(object_scanner_test, type_clob) {
    print(type::clob {});
}

TEST_F(object_scanner_test, type_unknown) {
    print(type::unknown {});
}

TEST_F(object_scanner_test, type_row_id) {
    print(type::row_id { 72 });
}

TEST_F(object_scanner_test, type_declared) {
    print(type::declared { typedesc("testing") });
}

TEST_F(object_scanner_test, type_extension) {
    print(type::dummy_extension { "testing" });
}

// scalar

TEST_F(object_scanner_test, scalar_immediate) {
    print(scalar::immediate {
            value::int4 { 100 },
            type::int4 {},
    });
}

TEST_F(object_scanner_test, scalar_variable_reference) {
    print(scalar::variable_reference { vardesc(1) });
}

TEST_F(object_scanner_test, scalar_unary) {
    print(scalar::unary {
            scalar::unary_operator::plus,
            scalar::variable_reference { vardesc(1) },
    });
}

TEST_F(object_scanner_test, scalar_cast) {
    print(scalar::cast {
            type::int4 {},
            scalar::cast::loss_policy_type::ignore,
            scalar::variable_reference { vardesc(1) },
    });
}

TEST_F(object_scanner_test, scalar_binary) {
    print(scalar::binary {
            scalar::binary_operator::add,
            scalar::variable_reference { vardesc(1) },
            scalar::variable_reference { vardesc(2) },
    });
}


TEST_F(object_scanner_test, scalar_compare) {
    print(scalar::compare {
            scalar::comparison_operator::equal,
            scalar::variable_reference { vardesc(1) },
            scalar::variable_reference { vardesc(2) },
    });
}

TEST_F(object_scanner_test, scalar_match) {
    print(scalar::match {
            scalar::match_operator::like,
            scalar::variable_reference { vardesc(1) },
            scalar::variable_reference { vardesc(2) },
            scalar::variable_reference { vardesc(3) },
    });
}

TEST_F(object_scanner_test, scalar_conditional) {
    print(scalar::conditional {
            {
                    scalar::conditional::alternative {
                            scalar::variable_reference { vardesc(1) },
                            scalar::variable_reference { vardesc(2) },
                    },
                    scalar::conditional::alternative {
                            scalar::variable_reference { vardesc(3) },
                            scalar::variable_reference { vardesc(4) },
                    },
                    scalar::conditional::alternative {
                            scalar::variable_reference { vardesc(5) },
                            scalar::variable_reference { vardesc(6) },
                    },
            },
            scalar::variable_reference { vardesc(7) },
    });
}

TEST_F(object_scanner_test, scalar_coalesce) {
    print(scalar::coalesce {
            {
                    scalar::variable_reference { vardesc(1) },
                    scalar::variable_reference { vardesc(2) },
                    scalar::variable_reference { vardesc(3) },
            },
    });
}

TEST_F(object_scanner_test, scalar_let) {
    print(scalar::let {
            {
                    scalar::let::declarator {
                            vardesc(1),
                            scalar::variable_reference { vardesc(2) },
                    },
                    scalar::let::declarator {
                            vardesc(3),
                            scalar::variable_reference { vardesc(4) },
                    },
                    scalar::let::declarator {
                            vardesc(5),
                            scalar::variable_reference { vardesc(6) },
                    },
            },
            scalar::variable_reference { vardesc(7) },
    });
}

TEST_F(object_scanner_test, scalar_function_call) {
    print(scalar::function_call {
            funcdesc(1),
            {
                    scalar::variable_reference { vardesc(2) },
                    scalar::variable_reference { vardesc(3) },
                    scalar::variable_reference { vardesc(4) },
            }
    });
}

TEST_F(object_scanner_test, scalar_extension) {
    print(scalar::dummy_extension {
            "Hello, world!"
    });
}

TEST_F(object_scanner_test, relation_find) {
    print(relation::find {
            tabledesc("A"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
            {
                    relation::find::key { vardesc(1), const_int4() },
                    relation::find::key { vardesc(3), const_int4() },
            }
    });
}

TEST_F(object_scanner_test, relation_scan) {
    print(relation::scan {
            tabledesc("A"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
            {
                    {
                            relation::scan::key { vardesc(1), const_int4() },
                            relation::scan::key { vardesc(3), const_int4() },
                    },
                    relation::endpoint_kind::inclusive,
            },
            {
                    {
                            relation::scan::key { vardesc(1), const_int4() },
                            relation::scan::key { vardesc(3), const_int4() },
                    },
                    relation::endpoint_kind::prefixed_exclusive,
            },
    });
}

TEST_F(object_scanner_test, relation_join_find) {
    print(relation::join_find {
            relation::join_kind::inner,
            tabledesc("A"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
            {
                    relation::join_find::key { vardesc(1), const_int4() },
                    relation::join_find::key{ vardesc(3), const_int4() },
            },
            const_boolean(),
    });
}

TEST_F(object_scanner_test, relation_join_scan) {
    print(relation::join_scan {
            relation::join_kind::inner,
            tabledesc("A"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
            {
                    {
                            relation::join_scan::key { vardesc(1), const_int4() },
                            relation::join_scan::key { vardesc(3), const_int4() },
                    },
                    relation::endpoint_kind::inclusive,
            },
            {
                    {
                            relation::join_scan::key { vardesc(1), const_int4() },
                            relation::join_scan::key { vardesc(3), const_int4() },
                    },
                    relation::endpoint_kind::prefixed_exclusive,
            },
            const_boolean(),
    });
}

TEST_F(object_scanner_test, relation_project) {
    print(relation::project {
            relation::project::column { vardesc(1), const_int4() },
            relation::project::column { vardesc(2), const_int4() },
            relation::project::column { vardesc(3), const_int4() },
    });
}

TEST_F(object_scanner_test, relation_filter) {
    print(relation::filter {
            const_boolean(),
    });
}

TEST_F(object_scanner_test, relation_buffer) {
    print(relation::buffer {
            3,
    });
}

TEST_F(object_scanner_test, relation_identify) {
    print(relation::identify {
            vardesc(1),
            type::row_id { 2 },
    });
}

TEST_F(object_scanner_test, relation_emit) {
    print(relation::emit {
            { vardesc(1), },
            { vardesc(2), "B" },
            { vardesc(3), "C" },
    });
}

TEST_F(object_scanner_test, relation_write) {
    print(relation::write {
            relation::write_kind::update,
            tabledesc("OUT"),
            {
                    relation::write::key  { vardesc(1), vardesc(2) },
            },
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
    });
}

TEST_F(object_scanner_test, relation_values) {
    print(relation::values {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { const_int4(), const_int4(), const_int4(), },
                    { const_int4(), const_int4(), const_int4(), },
                    { const_int4(), const_int4(), const_int4(), },
            },
    });
}

TEST_F(object_scanner_test, relation_join_relation) {
    using relation::intermediate::join;
    join expr {
            relation::join_kind::inner,
            const_boolean(),
    };
    expr.lower() = {
            {
                    join::key { vardesc(12), const_int4() },
                    join::key { vardesc(13), const_int4() },
            },
            relation::endpoint_kind::inclusive,
    };
    expr.upper() = {
            {
                    join::key { vardesc(12), const_int4() },
                    join::key { vardesc(13), const_int4() },
            },
            relation::endpoint_kind::inclusive,
    };
    print(expr);
}

TEST_F(object_scanner_test, relation_aggregate_relation) {
    print(relation::intermediate::aggregate {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("A"), {}, vardesc(4) },
                    { aggdesc("B"), { vardesc(10), }, vardesc(5) },
                    { aggdesc("C"), { vardesc(10), vardesc(20), vardesc(30), }, vardesc(6) },
            },
    });
}

TEST_F(object_scanner_test, relation_distinct_relation) {
    print(relation::intermediate::distinct {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    });
}

TEST_F(object_scanner_test, relation_limit_relation) {
    print(relation::intermediate::limit {
            1,
            {
                    vardesc(1),
                    vardesc(2),
            },
            {
                    vardesc(3),
                    { vardesc(4), relation::sort_direction::ascendant },
                    { vardesc(5), relation::sort_direction::descendant },
            }
    });
}

TEST_F(object_scanner_test, relation_union_relation) {
    print(relation::intermediate::union_ {
            {
                    { vardesc(1), vardesc(2), vardesc(3) },
                    { vardesc(4),         {}, vardesc(5) },
                    {         {}, vardesc(6), vardesc(7) },
            },
            relation::set_quantifier::all,
    });
}

TEST_F(object_scanner_test, relation_intersection_relation) {
    print(relation::intermediate::intersection {
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
            relation::set_quantifier::all,
    });
}

TEST_F(object_scanner_test, relation_difference_relation) {
    print(relation::intermediate::difference {
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            },
            relation::set_quantifier::distinct,
    });
}

TEST_F(object_scanner_test, relation_escape) {
    print(relation::intermediate::escape {
            { vardesc(1), vardesc(2) },
            { vardesc(3), vardesc(4) },
            { vardesc(5), vardesc(6) },
    });
}

TEST_F(object_scanner_test, relation_join_group) {
    print(relation::step::join {
            relation::join_kind::inner,
            const_boolean(),
    });
}

TEST_F(object_scanner_test, relation_aggregate_group) {
    print(relation::step::aggregate {
            {
                    { aggdesc("A"), {}, vardesc(4) },
                    { aggdesc("B"), { vardesc(10), }, vardesc(5) },
                    { aggdesc("C"), { vardesc(10), vardesc(20), vardesc(30), }, vardesc(6) },
            },
    });
}

TEST_F(object_scanner_test, relation_intersection_group) {
    print(relation::step::intersection {});
}

TEST_F(object_scanner_test, relation_difference_group) {
    print(relation::step::difference {});
}

TEST_F(object_scanner_test, relation_flatten_group) {
    print(relation::step::flatten {});
}

TEST_F(object_scanner_test, relation_take_flat) {
    print(relation::step::take_flat {
            exchangedesc("ex"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            }
    });
}

TEST_F(object_scanner_test, relation_take_group) {
    print(relation::step::take_group {
            exchangedesc("ex"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            }
    });
}

TEST_F(object_scanner_test, relation_take_cogroup) {
    print(relation::step::take_cogroup {
            {
                    exchangedesc("A"),
                    {
                            { vardesc(1), vardesc(2) },
                    }
            },
            {
                    exchangedesc("B"),
                    {
                            { vardesc(3), vardesc(4) },
                            { vardesc(5), vardesc(6) },
                    }
            },
            {
                    exchangedesc("C"),
                    {
                            { vardesc(7), vardesc(8) },
                            { vardesc(9), vardesc(10) },
                            { vardesc(11), vardesc(12) },
                    }
            },
    });
}

TEST_F(object_scanner_test, relation_offer) {
    print(relation::step::offer {
            exchangedesc("ex"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            }
    });
}

TEST_F(object_scanner_test, relation_graph) {
    relation::expression::graph_type r;
    auto&& r1 = r.insert(relation::scan {
            tabledesc("A"),
            {
                    { vardesc(1), vardesc(2) },
            },
    });
    auto&& r2 = r.insert(relation::emit {
            vardesc(2),
    });
    r1.output() >> r2.input();

    print(r);
}

TEST_F(object_scanner_test, plan_process) {
    plan::process step {};
    auto&& r1 = step.operators().insert(relation::scan {
            tabledesc("A"),
            {
                    { vardesc(1), vardesc(2) },
            },
    });
    auto&& r2 = step.operators().insert(relation::emit {
            vardesc(2),
    });
    r1.output() >> r2.input();

    print(step);
}

TEST_F(object_scanner_test, plan_forward) {
    print(plan::forward {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            1,
    });
}

TEST_F(object_scanner_test, plan_group) {
    print(plan::group {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
                    vardesc(4),
                    vardesc(5),
            },
            {
                    vardesc(1),
                    vardesc(2),
            },
            {
                    vardesc(3),
                    { vardesc(4), relation::sort_direction::ascendant },
                    { vardesc(5), relation::sort_direction::descendant },
            },
            1,
            plan::group_mode::equivalence,
    });
}

TEST_F(object_scanner_test, plan_aggregate) {
    print(plan::aggregate {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("A"), {}, vardesc(4) },
                    { aggdesc("B"), { vardesc(10), }, vardesc(5) },
                    { aggdesc("C"), { vardesc(10), vardesc(20), vardesc(30), }, vardesc(6) },
            },
            plan::group_mode::equivalence,
    });
}

TEST_F(object_scanner_test, plan_broadcast) {
    print(plan::broadcast {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    });
}

TEST_F(object_scanner_test, plan_discard) {
    print(plan::discard {});
}

TEST_F(object_scanner_test, plan_graph) {
    plan::graph_type p;
    auto&& p0 = p.insert(plan::process {});
    auto&& p1 = p.insert(plan::forward { vardesc(0) });
    auto&& p2 = p.insert(plan::process {});
    p0 >> p1;
    p1 >> p2;

    auto&& r0 = p0.operators().insert(relation::scan {
            tabledesc("A"),
            {
                    { vardesc(1), vardesc(2) },
            },
    });
    auto&& r1 = p0.operators().insert(relation::step::offer {
            exchangedesc("p1"),
            {
                    { vardesc(2), vardesc(0) },
            }
    });
    r0.output() >> r1.input();

    auto&& r2 = p2.operators().insert(relation::step::take_flat {
            exchangedesc("p1"),
            {
                    { vardesc(0), vardesc(3) },
            }
    });
    auto&& r3 = p2.operators().insert(relation::emit {
            vardesc(3),
    });
    r2.output() >> r3.input();

    print(p);
}

// statements

TEST_F(object_scanner_test, statement_execute) {
    statement::execute s;
    auto&& p = s.execution_plan();
    auto&& p0 = p.insert(plan::process {});

    auto&& r0 = p0.operators().insert(relation::scan {
            tabledesc("A"),
            {
                    { vardesc(1), vardesc(2) },
            },
    });
    auto&& r1 = p0.operators().insert(relation::emit {
            vardesc(2),
    });
    r0.output() >> r1.input();

    print(s);
}

TEST_F(object_scanner_test, statement_write) {
    print(statement::write {
            statement::write_kind::insert,
            tabledesc("T0"),
            {
                    columndesc("C0"),
                    columndesc("C1"),
            },
            {
                    {
                            scalar::variable_reference { vardesc(0) },
                            scalar::variable_reference { vardesc(1) },
                    },
            },
    });
}

TEST_F(object_scanner_test, create_table) {
    print(statement::create_table {
            schemadesc("S"),
            storagedesc("T0"),
            tabledesc("PK"),
            {
                    tabledesc("U0"),
                    tabledesc("U1"),
                    tabledesc("U2"),
            },
    });
}

TEST_F(object_scanner_test, drop_table) {
    print(statement::drop_table {
            schemadesc("S"),
            storagedesc("T0"),
    });
}

TEST_F(object_scanner_test, create_index) {
    print(statement::create_index {
            schemadesc("S"),
            tabledesc("T0"),
    });
}

TEST_F(object_scanner_test, drop_index) {
    print(statement::drop_index {
            schemadesc("S"),
            tabledesc("T0"),
    });
}

TEST_F(object_scanner_test, statement_empty) {
    print(statement::empty {});
}

TEST_F(object_scanner_test, statement_extension) {
    print(statement::dummy_extension {
            "Hello, world!"
    });
}

} // namespace takatori::serializer
