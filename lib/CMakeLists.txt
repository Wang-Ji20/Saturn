add_subdirectory(storage)
add_subdirectory(port)

add_library(
    saturn
    STATIC
    ${ALL_OBJECT_FILES}
)

set(
    SATURN_LIBS
    port
    storage
)

set(
    THIRD_PARTY_DEPENDENCIES
    absl::base
    absl::log
    absl::check
    absl::str_format
    absl::optional
    absl::statusor
    absl::status
)

target_link_libraries(
    saturn
    ${SATURN_LIBS}
    ${THIRD_PARTY_DEPENDENCIES}
)

target_include_directories(
  saturn PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
