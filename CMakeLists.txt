cmake_minimum_required(VERSION 3.11)

project(Saturn)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  if(NOT DEFINED WITH_IN_SOURCE_BUILD)
    message(FATAL_ERROR
      "CMake generation for saturn is not allowed within the source directory!"
      "\n Remove \"${CMAKE_SOURCE_DIR}/CMakeCache.txt\" and try again from another folder, e.g.:"
      "\n "
      "\n rm -rf CMakeCache.txt CMakeFiles"
      "\n cd .."
      "\n mkdir cmake-make"
      "\n cd cmake-make"
      "\n cmake ../saturn"
      "\n "
      "\n Alternately define WITH_IN_SOURCE_BUILD to force this option (not recommended!)"
    )
  endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(ABSL_PROPAGATE_CXX_STD True)


# if define ASAN, use address sanitizer
if(SANITIZER STREQUAL "ASAN")
    message(STATUS "Using ASAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

# if define TSAN, use thread sanitizer
if(SANITIZER STREQUAL "TSAN")
    message(STATUS "Using TSAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fno-omit-frame-pointer")
endif()

# include path
set(SATURN_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set(SATURN_THIRD_PARTY_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/third_party/abseil ${PROJECT_SOURCE_DIR}/third_party/yyjson/src)
include_directories(${SATURN_INCLUDE_DIRECTORY} ${SATURN_THIRD_PARTY_INCLUDE_DIRECTORIES})

add_subdirectory(lib)
add_subdirectory(third_party)

# if this project is included by other project, don't build test
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
