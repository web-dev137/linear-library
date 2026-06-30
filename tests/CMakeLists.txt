add_executable(LUTest
    LU_inplaceTest.cpp
)

add_executable(FlatMatrixTest
    FlatMatrixTest.cpp
)

target_link_libraries(LUTest PRIVATE
    LU_lib
    gtest_main
)

target_link_libraries(FlatMatrixTest PRIVATE
    matrix_lib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(LUTest)
gtest_discover_tests(FlatMatrixTest)