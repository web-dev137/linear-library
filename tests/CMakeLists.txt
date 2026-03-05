cmake_minimum_required(VERSION "4.2.3")

add_executable(tests
    MatrixTest.cpp
)

target_link_libraries(tests
    PRIVATE
    matrix_lib
    gtest_main
)

include(GoogleTest)

add_test(NAME tests COMMAND tests)

gtest_discover_tests(tests)