cmake_minimum_required(VERSION "4.2.3")

project(MatrixLib)
set(CMAKE_CXX_STANDART 17)

#====================== GoogleTest ================================
include(FetchContent)

FetchContent_Declare(
    googletest 
    GIT_REPOSITORY https://github.com/google/googletest.git 
    GIT_TAG        v1.17.0
    GIT_SHALLOW TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
#====================== GoogleTest End ============================

#====================== Add libraries ============================= 
add_library(LU_lib INTERFACE)
target_include_directories(LU_lib INTERFACE
    ${CMAKE_SOURCE_DIR}/linear-algebra/LU
)

add_library(vector_matrix_lib INTERFACE)
target_include_directories(vector_matrix_lib INTERFACE
    ${CMAKE_SOURCE_DIR}/linear-algebra/vector_matrix
)
#====================== Add libraries End =========================
enable_testing()


add_subdirectory(tests)
