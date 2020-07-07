set(test_target takatori-test)

add_executable(${test_target}
    main.cpp
)
target_include_directories(${test_target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${test_target}
    PUBLIC gtest
    PRIVATE takatori-impl
)

function (add_test_executable source_file)
    get_filename_component(test_name "${source_file}" NAME_WE)
    target_sources(${test_target}
        PRIVATE ${source_file}
    )
    add_test(
        NAME ${test_name}
        COMMAND ${test_target} --gtest_filter=${test_name}.* --gtest_output=xml:${test_name}_gtest_result.xml
    )
endfunction (add_test_executable)

# type models
add_test_executable(takatori/type/simple_type_test.cpp)
add_test_executable(takatori/type/decimal_type_test.cpp)
add_test_executable(takatori/type/character_type_test.cpp)
add_test_executable(takatori/type/bit_type_test.cpp)
add_test_executable(takatori/type/time_of_day_type_test.cpp)
add_test_executable(takatori/type/time_point_type_test.cpp)
add_test_executable(takatori/type/declared_type_test.cpp)
add_test_executable(takatori/type/extension_type_test.cpp)
add_test_executable(takatori/type/type_dispatch_test.cpp)

# value models
add_test_executable(takatori/value/simple_value_test.cpp)
add_test_executable(takatori/value/decimal_value_test.cpp)
add_test_executable(takatori/value/character_value_test.cpp)
add_test_executable(takatori/value/bit_value_test.cpp)
add_test_executable(takatori/value/extension_value_test.cpp)
add_test_executable(takatori/value/value_dispatch_test.cpp)

# datetime
add_test_executable(takatori/datetime/date_test.cpp)
add_test_executable(takatori/datetime/date_interval_test.cpp)
add_test_executable(takatori/datetime/time_of_day_test.cpp)
add_test_executable(takatori/datetime/time_interval_test.cpp)
add_test_executable(takatori/datetime/datetime_interval_test.cpp)
add_test_executable(takatori/datetime/time_point_test.cpp)
add_test_executable(takatori/datetime/time_zone_test.cpp)

# scalar expression models
add_test_executable(takatori/scalar/immediate_test.cpp)
add_test_executable(takatori/scalar/variable_reference_test.cpp)
add_test_executable(takatori/scalar/unary_test.cpp)
add_test_executable(takatori/scalar/cast_test.cpp)
add_test_executable(takatori/scalar/binary_test.cpp)
add_test_executable(takatori/scalar/compare_test.cpp)
add_test_executable(takatori/scalar/match_test.cpp)
add_test_executable(takatori/scalar/conditional_test.cpp)
add_test_executable(takatori/scalar/coalesce_test.cpp)
add_test_executable(takatori/scalar/let_test.cpp)
add_test_executable(takatori/scalar/function_call_test.cpp)
add_test_executable(takatori/scalar/extension_scalar_test.cpp)
add_test_executable(takatori/scalar/expression_dispatch_test.cpp)
add_test_executable(takatori/scalar/expression_walk_test.cpp)

# relational algebra expression models
add_test_executable(takatori/relation/find_test.cpp)
add_test_executable(takatori/relation/scan_test.cpp)
add_test_executable(takatori/relation/intermediate/join_relation_test.cpp)
add_test_executable(takatori/relation/join_find_test.cpp)
add_test_executable(takatori/relation/join_scan_test.cpp)
add_test_executable(takatori/relation/project_test.cpp)
add_test_executable(takatori/relation/filter_test.cpp)
add_test_executable(takatori/relation/buffer_test.cpp)
add_test_executable(takatori/relation/intermediate/aggregate_relation_test.cpp)
add_test_executable(takatori/relation/intermediate/distinct_relation_test.cpp)
add_test_executable(takatori/relation/intermediate/limit_relation_test.cpp)
add_test_executable(takatori/relation/intermediate/union_relation_test.cpp)
add_test_executable(takatori/relation/intermediate/intersection_relation_test.cpp)
add_test_executable(takatori/relation/intermediate/difference_relation_test.cpp)
add_test_executable(takatori/relation/emit_test.cpp)
add_test_executable(takatori/relation/write_test.cpp)
add_test_executable(takatori/relation/values_test.cpp)
add_test_executable(takatori/relation/intermediate/escape_test.cpp)
add_test_executable(takatori/relation/step/join_group_test.cpp)
add_test_executable(takatori/relation/step/aggregate_group_test.cpp)
add_test_executable(takatori/relation/step/intersection_group_test.cpp)
add_test_executable(takatori/relation/step/difference_group_test.cpp)
add_test_executable(takatori/relation/step/flatten_group_test.cpp)
add_test_executable(takatori/relation/step/take_flat_test.cpp)
add_test_executable(takatori/relation/step/take_group_test.cpp)
add_test_executable(takatori/relation/step/take_cogroup_test.cpp)
add_test_executable(takatori/relation/step/offer_test.cpp)
add_test_executable(takatori/relation/intermediate/relation_intermediate_dispatch_test.cpp)
add_test_executable(takatori/relation/step/relation_step_dispatch_test.cpp)
add_test_executable(takatori/relation/details/graph_merger_test.cpp)
add_test_executable(takatori/relation/relation_graph_test.cpp)

# step execution plan models
add_test_executable(takatori/plan/process_test.cpp)
add_test_executable(takatori/plan/exchange_test.cpp)
add_test_executable(takatori/plan/forward_test.cpp)
add_test_executable(takatori/plan/group_test.cpp)
add_test_executable(takatori/plan/aggregate_test.cpp)
add_test_executable(takatori/plan/broadcast_test.cpp)
add_test_executable(takatori/plan/discard_test.cpp)
add_test_executable(takatori/plan/plan_dispatch_test.cpp)
add_test_executable(takatori/plan/plan_graph_test.cpp)

# statement models
add_test_executable(takatori/statement/execute_test.cpp)
add_test_executable(takatori/statement/write_statement_test.cpp)
add_test_executable(takatori/statement/extension_statement_test.cpp)

# tree models
add_test_executable(takatori/tree/tree_element_test.cpp)
add_test_executable(takatori/tree/tree_element_util_test.cpp)
add_test_executable(takatori/tree/tree_element_vector_test.cpp)
add_test_executable(takatori/tree/tree_fragment_vector_test.cpp)
add_test_executable(takatori/tree/tree_element_forward_test.cpp)
add_test_executable(takatori/tree/tree_element_vector_forward_test.cpp)

# graph models
add_test_executable(takatori/graph/graph_test.cpp)
add_test_executable(takatori/graph/port_test.cpp)
add_test_executable(takatori/graph/multiport_test.cpp)
add_test_executable(takatori/graph/topological_sort_test.cpp)

# document models
add_test_executable(takatori/document/basic_document_test.cpp)
add_test_executable(takatori/document/region_test.cpp)
add_test_executable(takatori/document/document_map_test.cpp)

# serializer
add_test_executable(takatori/serializer/json_printer_test.cpp)
add_test_executable(takatori/serializer/object_scanner_test.cpp)

# utilities
add_test_executable(takatori/util/assertion_test.cpp)
add_test_executable(takatori/util/clonable_test.cpp)
add_test_executable(takatori/util/clonable_ptr_test.cpp)
add_test_executable(takatori/util/downcast_test.cpp)
add_test_executable(takatori/util/enum_set_test.cpp)
add_test_executable(takatori/util/exception_test.cpp)
add_test_executable(takatori/util/fail_test.cpp)
add_test_executable(takatori/util/finalizer_test.cpp)
add_test_executable(takatori/util/infect_qualifier_test.cpp)
add_test_executable(takatori/util/maybe_shared_ptr_test.cpp)
add_test_executable(takatori/util/object_creator_test.cpp)
add_test_executable(takatori/util/optional_ptr_test.cpp)
add_test_executable(takatori/util/ownership_reference_test.cpp)
add_test_executable(takatori/util/pointer_traits_test.cpp)
add_test_executable(takatori/util/sequence_view_test.cpp)
add_test_executable(takatori/util/reference_iterator_test.cpp)
add_test_executable(takatori/util/reference_list_view_test.cpp)
add_test_executable(takatori/util/reference_vector_test.cpp)
add_test_executable(takatori/util/rvalue_initializer_list_test.cpp)
add_test_executable(takatori/util/rvalue_reference_wrapper_test.cpp)
add_test_executable(takatori/util/smart_pointer_extractor_test.cpp)
add_test_executable(takatori/util/standard_memory_resource_test.cpp)
add_test_executable(takatori/util/static_bitset_test.cpp)
add_test_executable(takatori/util/string_builder_test.cpp)
add_test_executable(takatori/util/universal_extractor_test.cpp)
