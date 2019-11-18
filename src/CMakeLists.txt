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
    value/datetime_interval.cpp

    # datetime
    datetime/date.cpp
    datetime/time_of_day.cpp
    datetime/time_point.cpp
    datetime/date_interval.cpp
    datetime/time_interval.cpp
    datetime/datetime_interval.cpp
    datetime/time_zone.cpp
    datetime/time_zone_impl.cpp

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

    # scalar - util
    scalar/variable_declarator.cpp

    # relational
    relation/expression.cpp

    # relation - scan
    # relation/scan_full.cpp
    # relation/scan_point.cpp
    # relation/scan_prefix.cpp
    # relation/scan_range.cpp

    # relation - logical join
    # relation/logical/join_map.cpp
    # relation/logical/join_loop.cpp
    # relation/logical/join_merge.cpp

    # relation - join with scan
    # relation/join_scan_point.cpp
    # relation/join_scan_prefix.cpp
    # relation/join_scan_range.cpp

    # relation - tuple by tuple operations
    relation/select.cpp
    relation/select_column.cpp
    relation/where.cpp
    # relation/buffer.cpp

    # relation - logical grouping operations
    # relation/logical/aggregate.cpp
    # relation/logical/distinct.cpp
    # relation/logical/limit.cpp

    # relation - logical set operations
    # relation/logical/intersect.cpp
    # relation/logical/difference.cpp

    # relation - DML
    # relation/emit.cpp
    # relation/logical/emit_sorted.cpp
    # relation/insert.cpp
    # relation/update.cpp
    # relation/delete_.cpp
    # relation/overwrite.cpp

    # relation - misc.
    # relation/logical/union.cpp
    # relation/logical/escape.cpp

    # relation - physical join
    # relation/join_broadcast_map.cpp
    # relation/join_broadcast_loop.cpp
    # relation/join_group.cpp

    # relation - pyhisical grouping
    # relation/aggregate_group.cpp
    # relation/distinct_group.cpp
    # relation/limit_group.cpp
    # relation/intersect_group.cpp
    # relation/difference_group.cpp
    # relation/flatten.cpp

    # relation - communicate with exchange operators
    # relation/take_flat.cpp
    # relation/take_group.cpp
    # relation/take_cogroup.cpp
    # relation/offer.cpp

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
    PRIVATE ICU::uc
    PRIVATE ICU::data
    PRIVATE ICU::i18n
    PRIVATE ICU::io
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
