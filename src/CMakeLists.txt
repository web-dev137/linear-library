cmake_minimum_required(VERSION "4.2.3")

set(CMAKE_CXX_STANDART 14)

add_library(matrix_lib SHARED
    Matrix.hpp
)

set_target_properties(matrix_lib PROPERTIES LINKER_LANGUAGE CXX)