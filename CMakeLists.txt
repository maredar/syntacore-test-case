cmake_minimum_required(VERSION 3.18)

project(stc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(googletest)

add_executable(stc src/main.cpp)

enable_testing()

add_executable(
    stc_test
    tests/tests_main.cpp
)

target_link_libraries(
    stc_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(stc_test)
