cmake_minimum_required(VERSION 3.10)

project(Saturn)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include path
set(SATURN_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
include_directories(${SATURN_INCLUDE_DIRECTORY})

add_subdirectory(lib)
