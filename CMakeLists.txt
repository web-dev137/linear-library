cmake_minimum_required(VERSION "4.2.3")

project(MatrixLib)
set(CMAKE_CXX_STANDART 17)

set(CMAKE_AUTOMOC ON)
add_library(LU_lib INTERFACE)
target_include_directories(LU_lib INTERFACE
    ${CMAKE_SOURCE_DIR}/libs/LU
)

add_library(vector_matrix_lib INTERFACE)
target_include_directories(vector_matrix_lib INTERFACE
    ${CMAKE_SOURCE_DIR}/libs/vector_matrix
)

enable_testing()

add_subdirectory(external/googletest)
add_subdirectory(tests)
