add_executable(test_inplace_lu
    LU_inplaceTest.cpp
)

add_executable(test_flat_matrix
    FlatMatrixTest.cpp
)

target_link_libraries(test_inplace_lu
    PRIVATE
    LU_flat_lib
    gtest_main
)

target_link_libraries(test_flat_matrix
    PRIVATE
    flat_matrix_lib
    gtest_main
)

include(GoogleTest)


gtest_discover_tests(test_flat_matrix)
gtest_discover_tests(test_inplace_lu)