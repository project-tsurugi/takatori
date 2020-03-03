function (add_test_executable source_file)
    get_filename_component(test_name "${source_file}" NAME_WE)
    add_executable(${test_name}
        ${source_file}
    )
    target_include_directories(${test_name}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(${test_name}
        PRIVATE takatori
        PRIVATE takatori-impl
        PRIVATE takatori-test-main
        PRIVATE ICU::uc
        PRIVATE ICU::data
        PRIVATE ICU::i18n
        PRIVATE ICU::io
    )
    add_test(
        NAME ${test_name}
        COMMAND ${test_name} --gtest_output=xml:${test_name}_gtest_result.xml
    )
endfunction (add_test_executable)

add_library(takatori-test-main STATIC
    main.cpp
)

target_link_libraries(takatori-test-main
    PUBLIC gtest
)

# type models
add_test_executable(type/simple_type_test.cpp)
add_test_executable(type/decimal_type_test.cpp)
add_test_executable(type/character_type_test.cpp)
add_test_executable(type/bit_type_test.cpp)
add_test_executable(type/time_of_day_type_test.cpp)
add_test_executable(type/time_point_type_test.cpp)
add_test_executable(type/declared_type_test.cpp)
add_test_executable(type/extension_type_test.cpp)
add_test_executable(type/type_dispatch_test.cpp)

# value models
add_test_executable(value/simple_value_test.cpp)
add_test_executable(value/decimal_value_test.cpp)
add_test_executable(value/character_value_test.cpp)
add_test_executable(value/bit_value_test.cpp)
add_test_executable(value/value_dispatch_test.cpp)

# datetime
add_test_executable(datetime/date_test.cpp)
add_test_executable(datetime/date_interval_test.cpp)
add_test_executable(datetime/time_of_day_test.cpp)
add_test_executable(datetime/time_interval_test.cpp)
add_test_executable(datetime/datetime_interval_test.cpp)
add_test_executable(datetime/time_point_test.cpp)
add_test_executable(datetime/time_zone_test.cpp)

# scalar expression models
add_test_executable(scalar/immediate_test.cpp)
add_test_executable(scalar/variable_reference_test.cpp)
add_test_executable(scalar/unary_test.cpp)
add_test_executable(scalar/cast_test.cpp)
add_test_executable(scalar/binary_test.cpp)
add_test_executable(scalar/compare_test.cpp)
add_test_executable(scalar/match_test.cpp)
add_test_executable(scalar/conditional_test.cpp)
add_test_executable(scalar/coalesce_test.cpp)
add_test_executable(scalar/let_test.cpp)
add_test_executable(scalar/function_call_test.cpp)
add_test_executable(scalar/expression_dispatch_test.cpp)

# relational algebra expression models
add_test_executable(relation/find_test.cpp)
add_test_executable(relation/scan_test.cpp)
add_test_executable(relation/intermediate/join_relation_test.cpp)
add_test_executable(relation/join_find_test.cpp)
add_test_executable(relation/join_scan_test.cpp)
add_test_executable(relation/project_test.cpp)
add_test_executable(relation/filter_test.cpp)
add_test_executable(relation/buffer_test.cpp)
add_test_executable(relation/intermediate/aggregate_relation_test.cpp)
add_test_executable(relation/intermediate/distinct_relation_test.cpp)
add_test_executable(relation/intermediate/limit_relation_test.cpp)
add_test_executable(relation/intermediate/union_relation_test.cpp)
add_test_executable(relation/intermediate/intersection_relation_test.cpp)
add_test_executable(relation/intermediate/difference_relation_test.cpp)
add_test_executable(relation/emit_test.cpp)
add_test_executable(relation/write_test.cpp)
add_test_executable(relation/intermediate/escape_test.cpp)
add_test_executable(relation/step/join_group_test.cpp)
add_test_executable(relation/step/aggregate_group_test.cpp)
add_test_executable(relation/step/distinct_group_test.cpp)
add_test_executable(relation/step/limit_group_test.cpp)
add_test_executable(relation/step/intersection_group_test.cpp)
add_test_executable(relation/step/difference_group_test.cpp)
add_test_executable(relation/step/flatten_group_test.cpp)
add_test_executable(relation/step/take_flat_test.cpp)
add_test_executable(relation/step/take_group_test.cpp)
add_test_executable(relation/step/take_cogroup_test.cpp)
add_test_executable(relation/step/offer_test.cpp)
add_test_executable(relation/intermediate/relation_intermediate_dispatch_test.cpp)
add_test_executable(relation/step/relation_step_dispatch_test.cpp)
add_test_executable(relation/details/graph_merger_test.cpp)

# step execution plan models
add_test_executable(plan/process_test.cpp)
add_test_executable(plan/exchange_test.cpp)
add_test_executable(plan/forward_test.cpp)
add_test_executable(plan/group_test.cpp)
add_test_executable(plan/aggregate_test.cpp)
add_test_executable(plan/broadcast_test.cpp)
add_test_executable(plan/discard_test.cpp)
add_test_executable(plan/plan_dispatch_test.cpp)

# tree models
add_test_executable(tree/tree_element_test.cpp)
add_test_executable(tree/tree_element_util_test.cpp)
add_test_executable(tree/tree_element_vector_test.cpp)
add_test_executable(tree/tree_fragment_vector_test.cpp)
add_test_executable(tree/tree_element_forward_test.cpp)
add_test_executable(tree/tree_element_vector_forward_test.cpp)

# graph models
add_test_executable(graph/graph_test.cpp)
add_test_executable(graph/port_test.cpp)
add_test_executable(graph/multiport_test.cpp)

# document models
add_test_executable(document/basic_document_test.cpp)
add_test_executable(document/region_test.cpp)

# utilities
add_test_executable(util/clonable_test.cpp)
add_test_executable(util/clonable_ptr_test.cpp)
add_test_executable(util/consumer_test.cpp)
add_test_executable(util/downcast_test.cpp)
add_test_executable(util/enum_set_test.cpp)
add_test_executable(util/infect_qualifier_test.cpp)
add_test_executable(util/object_creator_test.cpp)
add_test_executable(util/optional_ptr_test.cpp)
add_test_executable(util/pointer_traits_test.cpp)
add_test_executable(util/sequence_view_test.cpp)
add_test_executable(util/reference_iterator_test.cpp)
add_test_executable(util/reference_list_view_test.cpp)
add_test_executable(util/reference_vector_test.cpp)
add_test_executable(util/rvalue_initializer_list_test.cpp)
add_test_executable(util/rvalue_reference_wrapper_test.cpp)
add_test_executable(util/smart_pointer_extractor_test.cpp)
add_test_executable(util/standard_memory_resource_test.cpp)
add_test_executable(util/universal_extractor_test.cpp)
