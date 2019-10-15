add_library(sandbox-cpp
    deleteme.cpp
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
