cmake_minimum_required(VERSION "4.2.3")

project(LinearLib)
set(CMAKE_CXX_STANDARD 17)

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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(vector_matrix_lib INTERFACE)
target_include_directories(vector_matrix_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(LU_lib INTERFACE vector_matrix_lib)

add_library( linear_lib INTERFACE)
target_link_libraries(linear_lib INTERFACE vector_matrix_lib LU_lib)
#====================== Add libraries End =========================

#====================== Tests =====================================
enable_testing()
add_subdirectory(tests)
# ===================== End tests =================================

#====================== Install ===================================
install(DIRECTORY include/ DESTINATION include)