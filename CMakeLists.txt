cmake_minimum_required(VERSION 3.10)

project(Saturn)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(ABSL_PROPAGATE_CXX_STD True)

# include path
set(SATURN_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set(SATURN_THIRD_PARTY_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/third_party/abseil)
include_directories(${SATURN_INCLUDE_DIRECTORY} ${SATURN_THIRD_PARTY_INCLUDE_DIRECTORIES})

add_subdirectory(lib)
add_subdirectory(third_party)
