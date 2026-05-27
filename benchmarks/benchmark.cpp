#include "benchmark/benchmark.h"
#include <random>
#include <linear-algebra/LU/DecomposeLU.hpp>
#include <linear-algebra/LU/LU.hpp>
#include <linear-algebra/LU/Inversion.hpp>
#include <linear-algebra/vector_matrix/VectorMatrix.hpp>
#include <linear-algebra/vector_matrix/FlatMatrix.hpp>

using namespace LinearAlgebra;
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


static void BM_Matrix_Multiply(benchmark::State &state,int size) {
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
BENCHMARK_CAPTURE(BM_Matrix_Multiply,1000, 1000);

static void BM_FlatMatrix_Multiply(benchmark::State &state, int size) {
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
BENCHMARK_CAPTURE(BM_FlatMatrix_Multiply,1000, 1000);

static void BM_Inv_Matrix(benchmark::State &state, int size) {
    for (auto _: state) {
        state.PauseTiming();
        auto Q = GenMatrix(size, 12345);
        auto LU = LinearAlgebra::DecomposeLU<double>(Q);
        state.ResumeTiming();
        auto C = LU.inv();
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}
BENCHMARK_CAPTURE(BM_Inv_Matrix,100, 100);
BENCHMARK_CAPTURE(BM_Inv_Matrix,500, 200);
BENCHMARK_CAPTURE(BM_Inv_Matrix,500, 500);
BENCHMARK_CAPTURE(BM_Inv_Matrix,1000, 1000);
BENCHMARK_CAPTURE(BM_Inv_Matrix, 2000, 2000);

static void BM_Inv_Flat_Matrix(benchmark::State &state,int size) {
    for (auto _:state) {
        state.PauseTiming();
        auto Q = GenFlatMatrix<double>(size,12345);
        auto LU = LinearAlgebra::LU<double,FlatMatrix<double>>(Q);
        auto I = LinearAlgebra::Inversion<double,FlatMatrix<double>>(LU);
        state.ResumeTiming();
        auto C = I.inv();
        benchmark::DoNotOptimize(C);
        benchmark::ClobberMemory();
    }
}


BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,100,100);
BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,200,200);
BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,500,500);
BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,1000,1000);
BENCHMARK_CAPTURE(BM_Inv_Flat_Matrix,2000,2000);
BENCHMARK_MAIN();