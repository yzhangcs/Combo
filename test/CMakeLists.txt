cmake_minimum_required(VERSION 2.8.2)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/googletest-download" )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/googletest-download" )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory("${CMAKE_CURRENT_BINARY_DIR}/googletest-src"
                 "${CMAKE_CURRENT_BINARY_DIR}/googletest-build")

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Add test executables
set(TEST_CPPLIB_LIST
    TestArrayDeque.cpp
    TestArrayQueue.cpp
    TestArrayStack.cpp
    TestLinkedDeque.cpp
    TestLinkedQueue.cpp
    TestLinkedStack.cpp
    TestList.cpp
    TestVector.cpp
    # TestBinaryHeap.cpp
    # TestIndexHeap.cpp
    # TestPriorityQueue.cpp
    # TestRandom.cpp
    # TestBinarySearchMap.cpp
    # TestBinarySearchSet.cpp
    # TestBinarySearchTreeMap.cpp
    # TestLinearProbingHashMap.cpp
    # TestRedBlackTreeMap.cpp
    # TestSeperateChainingHashMap.cpp
    # TestSequentialSearchMap.cpp
    # TestBubbleSort.cpp
    # TestHeapSort.cpp
    # TestInsertionSort.cpp
    # TestMergeSort.cpp
    # TestQuick3WaySort.cpp
    # TestQuickSort.cpp
    # TestSelectionSort.cpp
    # TestShellSort.cpp
    # TestQuickFind.cpp
    # TestQuickUnion.cpp
    # TestUnionFind.cpp
    # TestWeightedUnion.cpp
    )

# Link against gtest or gtest_main as needed.
foreach (src ${TEST_CPPLIB_LIST})
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src} ${CPPLIB_HEADERS})
    target_link_libraries(${name} gtest_main)
    add_test(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
endforeach ()

add_executable(Test ${TEST_CPPLIB_LIST} ${CPPLIB_HEADERS})
target_link_libraries(Test gtest_main)