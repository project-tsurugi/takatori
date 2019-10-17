add_library(sandbox-cpp

    # scalar
    model/scalar/expression.cpp

    # scalar - primary expressions
    model/scalar/immediate.cpp
    model/scalar/variable_reference.cpp

    # scalar - unary expressions
    model/scalar/unary.cpp
    model/scalar/cast.cpp

    # scalar - binary expressions
    model/scalar/binary.cpp
    model/scalar/compare.cpp

    # scalar - complex expression
    model/scalar/match.cpp
    model/scalar/conditional.cpp
    model/scalar/conditional_alternative.cpp
    model/scalar/coalesce.cpp
    model/scalar/let.cpp
    model/scalar/let_declarator.cpp
    model/scalar/function_call.cpp

    # util
    util/object_creator.cpp
)

add_executable(sandbox-cpp-exec
    main.cpp
)

target_include_directories(sandbox-cpp
    PRIVATE .
)

target_include_directories(sandbox-cpp-exec
    PRIVATE .
)

target_link_libraries(sandbox-cpp
    PUBLIC sandbox-cpp-api
)

if (USE_BOOST_MEMORY_RESOURCE)
    target_link_libraries(sandbox-cpp PUBLIC Boost::container)
    target_compile_definitions(sandbox-cpp PUBLIC USE_BOOST_MEMORY_RESOURCE)
endif (USE_BOOST_MEMORY_RESOURCE)

target_link_libraries(sandbox-cpp-exec
    PUBLIC sandbox-cpp
)

set_target_properties(sandbox-cpp
    PROPERTIES
        SOVERSION ${PROJECT_VERSION}
)

set_target_properties(sandbox-cpp-exec
    PROPERTIES
        SOVERSION ${PROJECT_VERSION}
)

install(
    TARGETS
        sandbox-cpp
        sandbox-cpp-exec
    EXPORT
        ${export_name}
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT Runtime
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${export_name}
        COMPONENT Development
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Runtime
)

# for tests
add_library(sandbox-cpp-impl INTERFACE)

target_include_directories(sandbox-cpp-impl
    INTERFACE .
)
