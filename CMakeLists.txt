cmake_minimum_required(VERSION "3.31.6")

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
add_library(vector_matrix_lib INTERFACE)
target_include_directories(vector_matrix_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(LU_lib INTERFACE)
target_include_directories(LU_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(LU_lib INTERFACE vector_matrix_lib)

add_library(LinearAlgebra INTERFACE)
add_library(LinearAlgebra::LinearAlgebra ALIAS LinearAlgebra)

target_link_libraries(LinearAlgebra INTERFACE 
    $<BUILD_INTERFACE:vector_matrix_lib>
    $<BUILD_INTERFACE:LU_lib>
)
#====================== Add libraries End =========================

#====================== Add doxygen ===============================
find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)
    
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        COMMENT "Generating API documentation with Doxygen"
    )
endif()
#====================== Tests =====================================
enable_testing()
add_subdirectory(tests)
# ===================== End tests =================================

#====================== Install ===================================
install(DIRECTORY include/ DESTINATION include)
install(TARGETS LinearAlgebra
    EXPORT LinearAlgebraTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES LinearAlgebraConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LinearAlgebra
)

install(EXPORT LinearAlgebraTargets
    FILE LinearAlgebraTargets.cmake
    NAMESPACE LinearAlgebra::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LinearAlgebra
)

option(BUILD_BENCHMARKS "Build benchmark targets" OFF)

if(BUILD_BENCHMARKS)
    add_executable(my_benchmark_exe benchmarks/benchmark.cpp)
    find_package(benchmark CONFIG REQUIRED)
    target_link_libraries(my_benchmark_exe PRIVATE 
        benchmark::benchmark 
        LinearAlgebra::LinearAlgebra
    )
endif()