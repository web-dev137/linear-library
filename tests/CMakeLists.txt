cmake_minimum_required(VERSION "3.31.6")

add_executable(test_lu
    LU_Test.cpp
)

add_executable(test_vector_matrix
    VectorMatrixTest.cpp
)

target_link_libraries(test_lu
    PRIVATE
    LU_lib
    gtest_main
)

target_link_libraries(test_vector_matrix
    PRIVATE
    vector_matrix_lib
    gtest_main
)

include(GoogleTest)

add_test(NAME test_lu COMMAND test_lu)
add_test(NAME test_vector_matrix COMMAND test_vector_matrix)

gtest_discover_tests(test_lu)
gtest_discover_tests(test_vector_matrix)