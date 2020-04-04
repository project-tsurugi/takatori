add_library(takatori

    # type
    type/data.cpp
    type/decimal.cpp
    type/character.cpp
    type/bit.cpp
    type/time_of_day.cpp
    type/time_point.cpp
    type/declared.cpp
    type/extension.cpp

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
    datetime/printing.cpp

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
    scalar/details/conditional_alternative.cpp
    scalar/coalesce.cpp
    scalar/let.cpp
    scalar/function_call.cpp

    # scalar - misc.

    # relational
    relation/expression.cpp

    # relation - scan
    relation/find.cpp
    relation/scan.cpp

    # relation - join
    relation/intermediate/join.cpp
    relation/join_find.cpp
    relation/join_scan.cpp

    # relation - tuple by tuple operations
    relation/project.cpp
    relation/filter.cpp
    relation/buffer.cpp

    # relation - grouping operations for whole relation
    relation/intermediate/aggregate.cpp
    relation/intermediate/distinct.cpp
    relation/intermediate/limit.cpp

    # relation - set operations for whole relation
    relation/intermediate/union.cpp
    relation/intermediate/intersection.cpp
    relation/intermediate/difference.cpp

    # relation - DML
    relation/emit.cpp
    relation/write.cpp

    # relation - misc.
    relation/graph.cpp
    relation/intermediate/escape.cpp

    # relation - dealing with group relations
    relation/step/join.cpp
    relation/step/aggregate.cpp
    relation/step/intersection.cpp
    relation/step/difference.cpp
    relation/step/flatten.cpp

    # relation - communicate with exchange operators
    relation/step/take_flat.cpp
    relation/step/take_group.cpp
    relation/step/take_cogroup.cpp
    relation/step/offer.cpp

    # relation - details
    relation/details/mapping_element.cpp
    relation/details/emit_element.cpp
    relation/details/sort_key_element.cpp
    relation/details/aggregate_element.cpp
    relation/details/key_pair_element.cpp
    relation/details/union_element.cpp
    relation/details/cogroup_element.cpp
    relation/details/graph_merger.cpp

    # plan
    plan/step.cpp
    plan/process.cpp
    plan/exchange.cpp
    plan/forward.cpp
    plan/group.cpp
    plan/aggregate.cpp
    plan/broadcast.cpp
    plan/discard.cpp
    plan/graph.cpp

    # document
    document/basic_document.cpp
    document/region.cpp

    # util
    util/object_creator.cpp
    util/string_builder.cpp
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

# Add INSTALL_RPATH from CMAKE_INSTALL_PREFIX and CMAKE_PREFIX_PATH
# The default behavior of CMake omits RUNPATH if it is already in CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES.
if (FORCE_INSTALL_RPATH)
    get_target_property(rpath takatori INSTALL_RPATH)

    # add ${CMAKE_INSTALL_PREFIX}/lib if it is not in system link directories
    get_filename_component(p "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" ABSOLUTE)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${p}" is_system)
    if (is_system STREQUAL "-1")
        list(APPEND rpath "${p}")
    endif()

    # add each ${CMAKE_PREFIX_PATH}/lib
    foreach (p IN LISTS CMAKE_PREFIX_PATH)
        get_filename_component(p "${p}/${CMAKE_INSTALL_LIBDIR}" ABSOLUTE)
        list(APPEND rpath "${p}")
    endforeach()

    if (rpath)
        set_target_properties(takatori
            PROPERTIES
                INSTALL_RPATH "${rpath}"
        )
    endif()

    # add other than */lib paths
    set_target_properties(takatori
        PROPERTIES
            INSTALL_RPATH_USE_LINK_PATH ON
    )
endif (FORCE_INSTALL_RPATH)

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
