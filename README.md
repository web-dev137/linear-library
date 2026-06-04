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
For other:
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
#include <decompose/LU/LU.hpp>
#include <matrix/FlatMatrix.hpp>
#include <memory>

int main() {
    using namespace LinearAlgebra;
    
    FlatMatrix<double> X = {
        {1,1},
        {1,2},
        {1,3},
        {1,4}
    };

    ColumnVector<double> y = {2,4,5,7};

    auto XT = ~X;
    auto XTX = XT*X;
    auto XTy = XT*y;
    LU<double> lu = LU<double>(std::move(XTX));
    auto Xinv = lu.inv();
    auto B = Xinv * XTy;
   
    std::cout << "Input data:\n";
    std::cout << "X: " << X << "\n";
    std::cout << "y:" << std::endl;
    for (int i = 0; i < y.size(); i++)
    {
        std::cout<<y[i]<<"\n";
    }
    std::cout << std::endl;

    std::cout << "Computing linear regression coefficients "
            << "B0 and B1 using the least squares method B = (X^T X)^-1 X^T y:\n";

     std::cout << "B0 = " << B[0] << "\n";
    std::cout << "B1 = " << B[1] << std::endl;
}
```