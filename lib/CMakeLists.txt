add_subdirectory(storage)
add_subdirectory(port)
add_subdirectory(common)

# -Wall -Werror
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

add_library(
    saturn
    STATIC
    ${ALL_OBJECT_FILES}
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
    yyjson
)

target_link_libraries(
    saturn
    ${THIRD_PARTY_DEPENDENCIES}
)

target_include_directories(
  saturn PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
