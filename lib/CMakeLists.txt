add_subdirectory(common)

add_library(
    saturn
    STATIC
    ${ALL_OBJECT_FILES}
)

set(
    SATURN_LIBS
    common
)

target_link_libraries(
    saturn
    ${SATURN_LIBS}
)

target_include_directories(
  saturn PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
