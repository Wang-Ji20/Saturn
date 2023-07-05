cmake_minimum_required(VERSION 3.11)

project(Saturn)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(ABSL_PROPAGATE_CXX_STD True)

# -Wall -Werror
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# include path
set(SATURN_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set(SATURN_THIRD_PARTY_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/third_party/abseil)
include_directories(${SATURN_INCLUDE_DIRECTORY} ${SATURN_THIRD_PARTY_INCLUDE_DIRECTORIES})

add_subdirectory(lib)
add_subdirectory(third_party)

# if this project is included by other project, don't build test
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
