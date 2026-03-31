cmake_minimum_required(VERSION "4.2.3")

add_executable(tests
    MatrixTest.cpp
)

add_executable(test_vector_matrix
    VectorMatrixTest.cpp
)

target_link_libraries(tests
    PRIVATE
    matrix_lib
    gtest_main
)

target_link_libraries(test_vector_matrix
    PRIVATE
    vector_matrix_lib
    gtest_main
)

include(GoogleTest)

add_test(NAME tests COMMAND tests)
add_test(NAME test_vector_matrix COMMAND test_vector_matrix)

gtest_discover_tests(tests)
gtest_discover_tests(test_vector_matrix)