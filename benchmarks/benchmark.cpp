#include "benchmark/benchmark.h"
#include <random>
#include <linear-algebra/LU/DecomposeLU.hpp>
#include <linear-algebra/LU/LU.hpp>
#include <linear-algebra/vector_matrix/VectorMatrix.hpp>
#include <linear-algebra/vector_matrix/FlatMatrix.hpp>

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

template<typename T>
LinearAlgebra::FlatMatrix<T> GenFlatMatrix(int size,int seed) {
    LinearAlgebra::FlatMatrix<T> matrix(size,size,T(0));

    std::mt19937 gen(seed);

    std::uniform_real_distribution<T> dis(0.0,1.0);
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
        {
            matrix(i,j) = dis(gen);
        }
    }
    
    return matrix;
}


static void BM_Matrix_Multiply(benchmark::State &state) {
    for (auto _:state) {
        state.PauseTiming();
        auto E = GenMatrix(1000,12345); 
        auto F = GenMatrix(1000,54321);
        state.ResumeTiming();
        auto C = E*F;
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_Matrix_Multiply);

static void BM_FlatMatrix_Multiply(benchmark::State &state) {
    for (auto _:state) {
        state.PauseTiming();
        auto E = GenFlatMatrix<double>(1000,12345); 
        auto F = GenFlatMatrix<double>(1000,54321);
        state.ResumeTiming();
        auto C = E*F;
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_FlatMatrix_Multiply);

static void BM_FlatMatrix_Transponse(benchmark::State &state) {
    for (auto _:state) {
        state.PauseTiming();
        auto E = GenFlatMatrix<double>(1000,12345); 
        state.ResumeTiming();
        auto C = ~E;
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_FlatMatrix_Transponse);

static void BM_VectorMatrix_Transponse(benchmark::State &state) {
    for (auto _:state) {
        state.PauseTiming();
        auto E = GenMatrix(1000,12345); 
        state.ResumeTiming();
        auto C = !E;
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_VectorMatrix_Transponse);

static void BM_LU(benchmark::State &state) {
    for (auto _:state) {
        state.PauseTiming();
        auto B = GenMatrix(2000,12345);
        state.ResumeTiming();
        auto LU = LinearAlgebra::DecomposeLU<double>(B);
        benchmark::DoNotOptimize(LU);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_LU);

static void BM_LU_Flat(benchmark::State &state) {
    for (auto _:state) {
        state.PauseTiming();
        auto B = GenFlatMatrix<double>(2000,12345);
        state.ResumeTiming();
        auto LU = LinearAlgebra::LU<double>(B);
        benchmark::DoNotOptimize(LU);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_LU_Flat);



static void BM_Inv_Matrix(benchmark::State &state) {
    for (auto _:state) {
        state.PauseTiming();
        auto Q = GenMatrix(1000,12345);
        auto LU = LinearAlgebra::DecomposeLU<double>(Q);
        state.ResumeTiming();
        auto C = LU.inv();
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_Inv_Matrix);
BENCHMARK_MAIN();