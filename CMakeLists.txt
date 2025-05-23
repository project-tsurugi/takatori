cmake_minimum_required(VERSION 3.16)

project(takatori-project
    VERSION 1.0.0
    DESCRIPTION "Takatori - A generic SQL intermediate representation"
    LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(BUILD_TESTS "build test programs" ON)
option(BUILD_DOCUMENTS "build documents" ON)
option(BUILD_SHARED_LIBS "build shared libraries instead of static" ON)
option(BUILD_STRICT "build with option strictly determine of success" ON)

option(FORCE_INSTALL_RPATH "automatically add library directory of custom prefixes to INSTALL_RPATH" OFF)

if (FORCE_INSTALL_RPATH)
    message(DEPRECATION "FORCE_INSTALL_RPATH is obsoleted")
endif (FORCE_INSTALL_RPATH)

option(ENABLE_COVERAGE "enable coverage on debug build" OFF)

include(GNUInstallDirs)

find_package(mpdecpp REQUIRED)
find_package(FLEX REQUIRED)
find_package(BISON 3.6 REQUIRED)

find_package(ICU 60
    COMPONENTS
        uc
        data
        i18n
        io
    REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.65
    COMPONENTS container
    OPTIONAL_COMPONENTS stacktrace_backtrace stacktrace_basic
    REQUIRED)
if(Boost_STACKTRACE_BACKTRACE_FOUND
   AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set(boost_stacktrace_component stacktrace_backtrace)
elseif(Boost_STACKTRACE_BASIC_FOUND)
    set(boost_stacktrace_component stacktrace_basic)
else()
    message(FATAL_ERROR "No usable Boost stacktrace component")
endif()

find_package(Doxygen
    OPTIONAL_COMPONENTS dot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (BUILD_STRICT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-sanitize=alignment")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-sanitize-recover=address")

if (ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
endif (ENABLE_COVERAGE)

set(export_name "takatori")
set(package_name "takatori")

include(CMakePackageConfigHelpers)

enable_testing()

add_library(takatori-api INTERFACE)

target_include_directories(takatori-api
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${export_name}>
)

add_subdirectory(src)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()
if (BUILD_DOCUMENTS)
    add_subdirectory(doxygen)
endif()
add_subdirectory(third_party)

install(
    TARGETS
        takatori-api
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

install(
    DIRECTORY include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${export_name}"
    COMPONENT Development
    FILES_MATCHING PATTERN "*.h"
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${package_name}-config.cmake
    @ONLY
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${package_name}-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${package_name}-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${package_name}-config-version.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/${package_name}
)

install(
    EXPORT ${package_name}
    FILE ${package_name}-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${package_name}
    EXPORT_LINK_INTERFACE_LIBRARIES
)
