include_directories(${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include)

include(GoogleTest)

add_executable(hello_test hello_test.cc)

gtest_discover_tests(hello_test
        EXTRA_ARGS
        --gtest_color=auto
        --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${bustub_test_name}.xml
        --gtest_catch_exceptions=0
        DISCOVERY_TIMEOUT 120
        PROPERTIES
        TIMEOUT 120
        )

target_link_libraries(hello_test saturn gtest gmock_main)
