cmake_minimum_required(VERSION 3.12.4)
project(Technopark_C_IZ2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic")
set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(matrix_static STATIC matrix.c)
add_library(matrix_dynamic SHARED matrix.c)

add_executable(Technopark_C_IZ2 main.c matrix.h matrix.c)
target_link_libraries(Technopark_C_IZ2 matrix_static matrix_dynamic)

add_executable(Technopark_C_IZ2_tests tests.cpp matrix.c)
target_link_libraries(Technopark_C_IZ2_tests gtest gtest_main pthread)