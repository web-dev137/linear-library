# Linear Algebra Library

A lightweight, header-only C++ library for linear algebra operations, focused on vector matrix, and common decompositions.

## Features

- **Matrix & Vector classes** with intuitive operator overloading
- Basic operations: transpose of matrix, multiplication,multiplication on scalar
- Advanced linear algebra methods:
  - LU Decomposition
  - Other matrix decompositions (in progress)
- Efficient memory management
- CMake build system support
- Unit tests included
- Add doxygen documentation is planned

## Project Structure
```bash
linear-library/
├── docs/                     # For doxygen docs
├── include/
│   └── linear-algebra/LU     # Header-only library source
│   └── linear-algebra/QR
│   └── linear-algebra/vector_matrix  
├── tests/                  # Test suite
├── CMakeLists.txt          # Build configuration
└── README.md
```
## Requirements

- **C++17** or higher
- CMake 3.10+

## Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/web-dev137/linear-library.git
   cd linear-library
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```
   Windows:
   ```bash
   mkdir build
   cd build
   cmake -G "MinGW Makefiles" ..
   make

## Generate docs 
cmake --build build --target doc

## Running Tests
After building, you can run the tests from the build directory
```bash
ctest
# or run the test executable directly if available
```

## Usage of example
```c++
#include <linear-algebra/LU/LU.hpp>
#include <linear-algebra/vector_matrix/VectorMatrix.hpp>

int main() {
    VectorMatrix<double> A;
    std::unique_ptr<DecomposeLU<double>> lu;
    A = VectorMatrix<double>({
            {0, 2, 1},
            {1, 1, 0},
            {2, 1, 1}
        });
    lu = std::make_unique<DecomposeLU<double>>(A);
    double det = lu->det();
}
