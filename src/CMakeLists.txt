add_library(takatori

    # type
    type/data.cpp
    type/decimal.cpp
    type/character.cpp
    type/bit.cpp
    type/time_of_day.cpp
    type/time_point.cpp

    # value
    value/data.cpp
    value/decimal.cpp
    value/character.cpp
    value/bit.cpp
    value/date.cpp
    value/time_of_day.cpp
    value/time_point.cpp
    value/time_interval.cpp

    # datetime
    datetime/date.cpp
    datetime/time_of_day.cpp
    datetime/time_point.cpp
    datetime/time_interval.cpp

    # scalar
    scalar/expression.cpp

    # scalar - primary expressions
    scalar/immediate.cpp
    scalar/variable_reference.cpp

    # scalar - unary expressions
    scalar/unary.cpp
    scalar/cast.cpp

    # scalar - binary expressions
    scalar/binary.cpp
    scalar/compare.cpp

    # scalar - complex expression
    scalar/match.cpp
    scalar/conditional.cpp
    scalar/conditional_alternative.cpp
    scalar/coalesce.cpp
    scalar/let.cpp
    scalar/let_declarator.cpp
    scalar/function_call.cpp

    # util
    util/object_creator.cpp
)

add_executable(takatori-exec
    main.cpp
)

target_include_directories(takatori
    PRIVATE .
)

target_include_directories(takatori-exec
    PRIVATE .
)

target_link_libraries(takatori
    PUBLIC takatori-api
    PUBLIC fpdecimal
    PUBLIC Boost::boost
)

if (USE_BOOST_MEMORY_RESOURCE)
    target_link_libraries(takatori PUBLIC Boost::container)
    target_compile_definitions(takatori PUBLIC USE_BOOST_MEMORY_RESOURCE)
endif (USE_BOOST_MEMORY_RESOURCE)

target_link_libraries(takatori-exec
    PUBLIC takatori
)

set_target_properties(takatori
    PROPERTIES
        SOVERSION ${PROJECT_VERSION}
)

set_target_properties(takatori-exec
    PROPERTIES
        SOVERSION ${PROJECT_VERSION}
)

install(
    TARGETS
        takatori
        takatori-exec
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
add_library(takatori-impl INTERFACE)

target_include_directories(takatori-impl
    INTERFACE .
)
