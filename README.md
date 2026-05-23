# Linear Algebra Library

A lightweight, header-only C++ library for linear algebra operations, focused on vector matrix, and common decompositions.

## Features

- **Matrix & Vector classes** with intuitive operator overloading
- Basic operations: transpose of matrix, multiplication,multiplication on scalar
- Advanced linear algebra methods:
  - LU Decomposition+determinant+inversion matrix
  - Other matrix decompositions (in progress)
- Efficient memory management
- CMake build system support
- Unit tests included
- Doxygen documentation(https://web-dev137.github.io/linear-library/annotated.html)

## Requirements

- **C++17** or higher
- CMake 3.10+

## Running Tests
After building, you can run the tests from the build directory
```bash
ctest
# or run the test executable directly if available
```
# Usage
## Fetch package
In your project:
```cmake
include(FetchContent)
FetchContent_Declare(LinearAlgebra GIT_REPOSITORY https://github.com/... GIT_TAG main)
FetchContent_MakeAvailable(LinearAlgebra)
```
## Add subdirectory 
Clone library in to your project for example in external folder
```cmake
cmake_minimum_required(VERSION 3.14)
project(RandomMathApp LANGUAGES CXX)


add_subdirectory(external/linear-library)

add_executable(random_math_app main.cpp)

target_link_libraries(random_math_app PRIVATE LinearAlgebra::LinearAlgebra)
target_compile_features(random_math_app PRIVATE cxx_std_17)
```
## Install + Find  package
Build and install library local:
```bash
git clone https://github.com/web-dev137/linear-library.git
cd linear-library
```
For windows:
```bash
cmake -B build -S .  -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"
```
Fo other:
```bash
cmake -B build -S .  -DCMAKE_BUILD_TYPE=Release
```
```bash
cmake --build build --target all
cmake --install build --prefix /path/to/lib_install
```

After include library in your cmake file:
```cmake
cmake_minimum_required(VERSION 3.14)
project(RandomMathApp LANGUAGES CXX)

find_package(LinearAlgebra REQUIRED)

add_executable(random_math_app main.cpp)

target_link_libraries(random_math_app PRIVATE LinearAlgebra::LinearAlgebra)
target_compile_features(random_math_app PRIVATE cxx_std_17)
```

## Usage of example
```c++
#include <iostream>
#include <linear-algebra/LU/LU.hpp>
#include <memory>

using namespace LinearAlgebra;

int main() {
 
    auto A = VectorMatrix<double>({
        {2.0, 5.0, 7.0},
        {6.0, 3.0, 4.0},
        {5.0, -2.0, -3.0}
    });

    auto lu = DecomposeLU<double>(A);

    std::cout << "\ndet:\n" << lu.det();
}
```
