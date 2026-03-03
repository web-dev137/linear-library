cmake_minimum_required(VERSION "4.2.3")

set(CMAKE_CXX_STANDART 14)


add_executable(app
    main.cpp
)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
target_link_libraries(app PRIVATE matrix_lib)