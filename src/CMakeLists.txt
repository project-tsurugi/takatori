set(FLEX_FLAGS_DEBUG "-d")
set(FLEX_FLAGS_RELEASE "-Cfe")
set(FLEX_FLAGS_RELWITHDEBINFO "-Cfe")
set(FLEX_FLAGS_MINSIZEREL "-Ce")

set(BISON_FLAGS_DEBUG "-Dparse.assert -Dparse.trace")
set(BISON_FLAGS_RELEASE "")
set(BISON_FLAGS_RELWITHDEBINFO "")
set(BISON_FLAGS_MINSIZEREL "")

string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_CMAKE_BUILD_TYPE)
set(FLEX_FLAGS "${FLEX_FLAGS_${UPPER_CMAKE_BUILD_TYPE}}")
set(BISON_FLAGS "-Wall -Werror -Wno-error=conflicts-sr -ra ${BISON_FLAGS_${UPPER_CMAKE_BUILD_TYPE}}")

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/takatori/datetime/parser)

FLEX_TARGET(datetime_scanner
    takatori/datetime/parser/scanner.ll
    ${CMAKE_CURRENT_BINARY_DIR}/takatori/datetime/parser/scanner_generated.cpp
    COMPILE_FLAGS "${FLEX_FLAGS}"
)

BISON_TARGET(datetime_parser
    takatori/datetime/parser/parser.yy
    ${CMAKE_CURRENT_BINARY_DIR}/takatori/datetime/parser/parser_generated.cpp
    COMPILE_FLAGS "${BISON_FLAGS}"
    VERBOSE REPORT_FILE ${CMAKE_CURRENT_BINARY_DIR}/takatori/datetime/parser/parser_report.log
)

ADD_FLEX_BISON_DEPENDENCY(
    datetime_scanner
    datetime_parser
)

set_source_files_properties(
    ${BISON_datetime_parser_OUTPUTS}
    PROPERTIES
    COMPILE_OPTIONS -Wno-unused-but-set-variable
)

add_library(takatori

    # name
    takatori/name/identifier.cpp
    takatori/name/name.cpp

    # type
    takatori/type/data.cpp
    takatori/type/decimal.cpp
    takatori/type/character.cpp
    takatori/type/octet.cpp
    takatori/type/bit.cpp
    takatori/type/time_of_day.cpp
    takatori/type/time_point.cpp
    takatori/type/row_id.cpp
    takatori/type/declared.cpp
    takatori/type/extension.cpp

    # value
    takatori/value/primitive.cpp
    takatori/value/data.cpp
    takatori/value/decimal.cpp
    takatori/value/character.cpp
    takatori/value/octet.cpp
    takatori/value/bit.cpp
    takatori/value/date.cpp
    takatori/value/time_of_day.cpp
    takatori/value/time_point.cpp
    takatori/value/datetime_interval.cpp
    takatori/value/extension.cpp

    # decimal
    takatori/decimal/triple.cpp

    # datetime
    takatori/datetime/date.cpp
    takatori/datetime/time_of_day.cpp
    takatori/datetime/time_point.cpp
    takatori/datetime/date_interval.cpp
    takatori/datetime/time_interval.cpp
    takatori/datetime/datetime_interval.cpp
    takatori/datetime/time_zone.cpp
    takatori/datetime/time_zone_impl.cpp
    takatori/datetime/conversion.cpp
    takatori/datetime/printing.cpp

    # datetime parser
    takatori/datetime/parser/region.cpp
    takatori/datetime/parser/driver.cpp
    takatori/datetime/parser/scanner.cpp
    takatori/datetime/parser/parser.cpp
    ${FLEX_datetime_scanner_OUTPUTS}
    ${BISON_datetime_parser_OUTPUTS}

    # scalar
    takatori/scalar/expression.cpp

    # scalar - primary expressions
    takatori/scalar/immediate.cpp
    takatori/scalar/variable_reference.cpp

    # scalar - unary expressions
    takatori/scalar/unary.cpp
    takatori/scalar/cast.cpp

    # scalar - binary expressions
    takatori/scalar/binary.cpp
    takatori/scalar/compare.cpp

    # scalar - complex expression
    takatori/scalar/match.cpp
    takatori/scalar/conditional.cpp
    takatori/scalar/details/conditional_alternative.cpp
    takatori/scalar/coalesce.cpp
    takatori/scalar/let.cpp
    takatori/scalar/function_call.cpp

    # scalar - misc.
    takatori/scalar/extension.cpp

    # relational
    takatori/relation/expression.cpp

    # relation - scan
    takatori/relation/find.cpp
    takatori/relation/scan.cpp

    # relation - join
    takatori/relation/intermediate/join.cpp
    takatori/relation/join_find.cpp
    takatori/relation/join_scan.cpp

    # relation - tuple by tuple operations
    takatori/relation/project.cpp
    takatori/relation/filter.cpp
    takatori/relation/buffer.cpp
    takatori/relation/identify.cpp

    # relation - grouping operations for whole relation
    takatori/relation/intermediate/aggregate.cpp
    takatori/relation/intermediate/distinct.cpp
    takatori/relation/intermediate/limit.cpp

    # relation - set operations for whole relation
    takatori/relation/intermediate/union.cpp
    takatori/relation/intermediate/intersection.cpp
    takatori/relation/intermediate/difference.cpp

    # relation - DML
    takatori/relation/emit.cpp
    takatori/relation/write.cpp

    # relation - misc.
    takatori/relation/graph.cpp
    takatori/relation/values.cpp
    takatori/relation/intermediate/escape.cpp

    # relation - dealing with group relations
    takatori/relation/step/join.cpp
    takatori/relation/step/aggregate.cpp
    takatori/relation/step/intersection.cpp
    takatori/relation/step/difference.cpp
    takatori/relation/step/flatten.cpp

    # relation - communicate with exchange operators
    takatori/relation/step/take_flat.cpp
    takatori/relation/step/take_group.cpp
    takatori/relation/step/take_cogroup.cpp
    takatori/relation/step/offer.cpp

    # relation - details
    takatori/relation/details/mapping_element.cpp
    takatori/relation/details/emit_element.cpp
    takatori/relation/details/sort_key_element.cpp
    takatori/relation/details/aggregate_element.cpp
    takatori/relation/details/key_pair_element.cpp
    takatori/relation/details/values_row.cpp
    takatori/relation/details/union_element.cpp
    takatori/relation/details/cogroup_element.cpp
    takatori/relation/details/graph_merger.cpp

    # plan
    takatori/plan/step.cpp
    takatori/plan/process.cpp
    takatori/plan/exchange.cpp
    takatori/plan/forward.cpp
    takatori/plan/group.cpp
    takatori/plan/aggregate.cpp
    takatori/plan/broadcast.cpp
    takatori/plan/discard.cpp
    takatori/plan/graph.cpp

    # statement
    takatori/statement/statement.cpp
    takatori/statement/execute.cpp
    takatori/statement/write.cpp
    takatori/statement/create_table.cpp
    takatori/statement/create_index.cpp
    takatori/statement/drop_table.cpp
    takatori/statement/drop_index.cpp
    takatori/statement/empty.cpp
    takatori/statement/extension.cpp
    takatori/statement/details/write_tuple.cpp

    # document
    takatori/document/basic_document.cpp
    takatori/document/region.cpp
    takatori/document/document_map.cpp

    # serializer
    takatori/serializer/json_printer.cpp
    takatori/serializer/object_scanner.cpp
    takatori/serializer/details/simple_value_scanner.cpp
    takatori/serializer/details/value_property_scanner.cpp
    takatori/serializer/details/type_property_scanner.cpp
    takatori/serializer/details/scalar_expression_property_scanner.cpp
    takatori/serializer/details/relation_expression_property_scanner.cpp
    takatori/serializer/details/step_property_scanner.cpp
    takatori/serializer/details/statement_property_scanner.cpp

    takatori/serializer/value_input.cpp
    takatori/serializer/value_output.cpp
    takatori/serializer/value_input_exception.cpp
    takatori/serializer/base128v.cpp

    # util
    takatori/util/assertion.cpp
    takatori/util/exception.cpp
    takatori/util/fail.cpp
    takatori/util/string_builder.cpp
)

target_include_directories(takatori
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE ${FLEX_INCLUDE_DIRS}
)

target_link_libraries(takatori
    PUBLIC takatori-api
    PUBLIC mpdecpp
    PUBLIC Boost::boost
    PRIVATE ICU::uc
    PRIVATE ICU::data
    PRIVATE ICU::i18n
    PRIVATE ICU::io
)

# FIXME: move to common core module
target_compile_definitions(takatori
    PUBLIC BOOST_ENABLE_ASSERT_DEBUG_HANDLER
)

if (boost_stacktrace_component STREQUAL "stacktrace_backtrace")
    target_link_libraries(takatori
        PUBLIC Boost::stacktrace_backtrace
        PUBLIC ${CMAKE_DL_LIBS}
    )
    target_compile_definitions(takatori
        PUBLIC BOOST_STACKTRACE_USE_BACKTRACE
    )
else()
    target_link_libraries(takatori
        PUBLIC Boost::stacktrace_basic
        PUBLIC ${CMAKE_DL_LIBS}
    )
endif()

if (ENABLE_PMR)
    target_compile_definitions(takatori PUBLIC ENABLE_OBJECT_CREATOR_PMR)
endif (ENABLE_PMR)

if (USE_BOOST_MEMORY_RESOURCE)
    target_link_libraries(takatori PUBLIC Boost::container)
    target_compile_definitions(takatori PUBLIC USE_BOOST_MEMORY_RESOURCE)
endif (USE_BOOST_MEMORY_RESOURCE)

set_target_properties(takatori
    PROPERTIES
        INSTALL_RPATH "\$ORIGIN"
)

install(
    TARGETS
        takatori
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

target_link_libraries(takatori-impl
    INTERFACE takatori
    INTERFACE ICU::uc
    INTERFACE ICU::data
    INTERFACE ICU::i18n
    INTERFACE ICU::io
)