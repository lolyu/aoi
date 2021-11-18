cmake_minimum_required(VERSION 3.10)
project(gtest_project)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
# https://cmake.org/cmake/help/latest/command/add_executable.html
# cmake CMakeLists.txt
# make
# ./runTests
add_executable(runTests hello_test.cc)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)