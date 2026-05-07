#include "benchmark/benchmark.h"
#include <random>
#include <linear-algebra/LU/DecomposeLU.hpp>
#include <linear-algebra/vector_matrix/VectorMatrix.hpp>

LinearAlgebra::VectorMatrix<double> GenMatrix(int size,int seed) {
    LinearAlgebra::VectorMatrix<double> matrix(size,size);

    std::mt19937 gen(seed);

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
    auto E = GenMatrix(1000,12345); 
    auto F = GenMatrix(1000,54321);
    for (auto _:state) {
        auto C = E*F;
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_Matrix_Multiply);


static void BM_LU(benchmark::State &state) {
    auto B = GenMatrix(1000,12345);
    for (auto _:state) {
        auto LU = LinearAlgebra::DecomposeLU<double>(B);
        benchmark::DoNotOptimize(LU);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_LU);

static void BM_Inv_Matrix(benchmark::State &state) {
    auto Q = GenMatrix(1000,12345);
    for (auto _:state) {
        auto LU = LinearAlgebra::DecomposeLU<double>(Q);
        auto C = LU.inv();
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_Inv_Matrix);
BENCHMARK_MAIN();