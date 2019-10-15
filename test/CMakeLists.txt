function(add_test_executable source_file)
    get_filename_component(test_name "${source_file}" NAME_WE)
    add_executable(${test_name}
        ${source_file}
    )
    target_include_directories(${test_name}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(${test_name}
        PRIVATE sandbox-cpp
        PRIVATE sandbox-cpp-impl
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

add_test_executable(util/optional_view_test.cpp)
