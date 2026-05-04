#include "benchmark/benchmark.h"
#include <random>
#include <linear-algebra/LU/DecomposeLU.hpp>
#include <linear-algebra/vector_matrix/VectorMatrix.hpp>

LinearAlgebra::VectorMatrix<double> GenMatrix(int size) {
    LinearAlgebra::VectorMatrix<double> matrix(size,size);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dis(0.0,1.0);
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
        {
            matrix(i,j) = dis(gen);
        }
    }
    
    return matrix;
}


static void BM_Matrix_Multiply(benchmark::State &state) {
    auto A = GenMatrix(10);
    auto B = GenMatrix(10);

    //LinearAlgebra::VectorMatrix<double> C(3,3);
    for (auto _:state) {
        auto C = A*B;
        benchmark::DoNotOptimize(C);
    }
}
BENCHMARK(BM_Matrix_Multiply);


static void BM_LU(benchmark::State &state) {
    auto A = GenMatrix(1000);
    for (auto _:state) {
        auto LU = LinearAlgebra::DecomposeLU<double>(A);
        benchmark::DoNotOptimize(LU);
    }
}

BENCHMARK(BM_LU);

static void BM_Inv_Matrix(benchmark::State &state) {
    auto A = GenMatrix(1000);

    for (auto _:state) {
        auto LU = LinearAlgebra::DecomposeLU<double>(A);
        auto C = LU.inv();
        benchmark::DoNotOptimize(C);
    }
}

BENCHMARK(BM_Inv_Matrix);
BENCHMARK_MAIN();