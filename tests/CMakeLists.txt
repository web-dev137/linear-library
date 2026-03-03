cmake_minimum_required(VERSION "4.2.3")
# Include the GoogleTest module

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main # Use a specific tag or branch
)
# This will populate the googletest content and make targets available
FetchContent_MakeAvailable(googletest)


add_executable(tests
    MatrixTest.cpp
)

target_link_libraries(tests
    PRIVATE
    matrix_lib
    gtest_main
)

include(GoogleTest)

add_test(NAME tests COMMAND tests)

gtest_discover_tests(tests)