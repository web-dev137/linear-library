#include "benchmark/benchmark.h"
#include <random>
#include <decompose/LU/LU.hpp>
#include <matrix/FlatMatrix.hpp>

using namespace LinearAlgebra;

template<typename T>
LinearAlgebra::FlatMatrix<T> GenFlatMatrix(int size,int seed) {
    LinearAlgebra::FlatMatrix<T> matrix(size,size);

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


static void BM_Inv_Flat_Matrix(benchmark::State &state,int size) {
    for (auto _:state) {
        state.PauseTiming();
        auto Q = GenFlatMatrix<double>(size,12345);
        auto lu = LinearAlgebra::LU<double,FlatMatrix<double>>(std::move(Q));
        state.ResumeTiming();
        auto C = lu.inv();
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}


BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,100,100);
BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,200,200);
BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,500,500);
BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,1000,1000);
BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,2000,2000);

static void BM_FlatMatrix_Multiply(benchmark::State &state, int size) {
    for (auto _:state) {
        state.PauseTiming();
        auto E = GenFlatMatrix<double>(size,12345); 
        auto F = GenFlatMatrix<double>(size,54321);
        state.ResumeTiming();
        auto C = E*F;
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}
BENCHMARK_CAPTURE(BM_FlatMatrix_Multiply,1000, 1000);
BENCHMARK_MAIN();