include_directories(${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include)

include(GoogleTest)

add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

file(GLOB_RECURSE SATURN_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cc")

message(STATUS "Found ${SATURN_TEST_SOURCES} test sources: ${PROJECT_SOURCE_DIR}/test/")

foreach (saturn_test_source ${SATURN_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(saturn_test_filename ${saturn_test_source} NAME)
    string(REPLACE ".cc" "" saturn_test_name ${saturn_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${saturn_test_name} EXCLUDE_FROM_ALL ${saturn_test_source})
    add_dependencies(build-tests ${saturn_test_name})
    add_dependencies(check-tests ${saturn_test_name})

    gtest_discover_tests(${saturn_test_name}
            EXTRA_ARGS
            --gtest_color=auto
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${saturn_test_name}.xml
            --gtest_catch_exceptions=0
            DISCOVERY_TIMEOUT 120
            PROPERTIES
            TIMEOUT 120
            )

    target_link_libraries(${saturn_test_name} saturn gtest gmock_main)

    # Set test target properties and dependencies.
    set_target_properties(${saturn_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${saturn_test_name}
            )
endforeach ()
