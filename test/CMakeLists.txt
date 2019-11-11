function(add_test_executable source_file)
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
        PRIVATE gtest
    )
    target_sources(${test_name}
        PRIVATE main.cpp
    )
    add_test(
        NAME ${test_name}
        COMMAND ${test_name} --gtest_output=xml:${test_name}_gtest_result.xml
    )
endfunction(add_test_executable)

add_test_executable(scalar/expression_dispatch_test.cpp)

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

add_test_executable(tree/tree_element_test.cpp)
add_test_executable(tree/tree_element_util_test.cpp)
add_test_executable(tree/tree_element_vector_test.cpp)
add_test_executable(tree/tree_fragment_vector_test.cpp)

add_test_executable(tree/tree_element_forward_test.cpp)
add_test_executable(tree/tree_element_vector_forward_test.cpp)

add_test_executable(util/clonable_test.cpp)
add_test_executable(util/clonable_ptr_test.cpp)
add_test_executable(util/downcast_test.cpp)
add_test_executable(util/infect_qualifier_test.cpp)
add_test_executable(util/object_creator_test.cpp)
add_test_executable(util/optional_ptr_test.cpp)
add_test_executable(util/poiner_traits_test.cpp)
add_test_executable(util/reference_iterator_test.cpp)
add_test_executable(util/reference_vector_test.cpp)
add_test_executable(util/rvalue_initializer_list_test.cpp)
add_test_executable(util/rvalue_reference_wrapper_test.cpp)
add_test_executable(util/standard_memory_resource_test.cpp)
