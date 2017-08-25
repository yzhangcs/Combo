cmake_minimum_required(VERSION 2.8.2)

project(CppLib)

# Disable in-source build
if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    file(MAKE_DIRECTORY build)
    message(FATAL_ERROR "In-source builds are not allowed.\n
                         Please try:\n
                         1.Delete 'CMakeFiles' and 'CMakeCache.txt' in current directory\n
                         2.Change to directory 'build' and run 'cmake ..'")
endif ()

# Set default build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

# Options
option(CPPLIB_BUILD_TEST "Build CppLib tests." OFF)

# Compiler config
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -std=c++11 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif ()
message(STATUS "CMAKE_BUILD_TYPE:        ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER_ID:   ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_FLAGS:         ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG:   ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Include headers
file(GLOB_RECURSE CPPLIB_HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")
include_directories(${PROJECT_SOURCE_DIR}/include)

# Include test subdirectory
if (CPPLIB_BUILD_TEST)
    # include(CTest)
    enable_testing()
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif ()

# Add executables
set(CPPLIB_EXEC_LIST
    # Deque
    # Heap
    # List
    # PriorityQueue
    # Queue
    # Random
    # Search
    # Sort
    # Stack
    Timer
    # UnionFind
    )

foreach (exec ${CPPLIB_EXEC_LIST})
    add_executable(${exec} ${PROJECT_SOURCE_DIR}/src/${exec}.cpp ${CPPLIB_HEADERS})
endforeach ()

add_custom_target(run
    COMMAND ./bin/Stack ./data/tobe.txt
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)