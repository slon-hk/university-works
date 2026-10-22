cmake_minimum_required(VERSION 3.14)
project(timsort_lb2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
add_compile_options(-Wall -Wextra)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(MODULES ${SRC}/modules)

add_executable(timsort ${SRC}/main.cpp)

add_executable(benchmark ${MODULES}/benchmark.cpp)

find_package(GTest REQUIRED)
add_executable(tests ${SRC}/tests.cpp)
target_link_libraries(tests GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)

add_custom_target(bench
    COMMAND benchmark > ${MODULES}/results.csv
    DEPENDS benchmark
    COMMENT "Running benchmark -> src/modules/results.csv"
)

find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    add_custom_target(plot
        COMMAND ${Python3_EXECUTABLE} ${MODULES}/plot_benchmark.py
        COMMENT "Building plots -> src/modules/plots"
    )
endif()
