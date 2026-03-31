cmake_minimum_required(VERSION "4.2.3")

project(MatrixApp)
set(CMAKE_CXX_STANDART 17)

set(CMAKE_AUTOMOC ON)
add_library(matrix_lib INTERFACE)
target_include_directories(matrix_lib INTERFACE
    ${CMAKE_SOURCE_DIR}/libs/matrix
)

add_library(vector_matrix_lib INTERFACE)
target_include_directories(vector_matrix_lib INTERFACE
    ${CMAKE_SOURCE_DIR}/libs/vector_matrix
)
add_executable(MatrixApp
    src/main.cpp
)
target_include_directories(MatrixApp PRIVATE ${CMAKE_SOURCE_DIR}/include)

enable_testing()



add_subdirectory(external/googletest)
add_subdirectory(tests)

target_link_libraries(MatrixApp PRIVATE  matrix_lib)
