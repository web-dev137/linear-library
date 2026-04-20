#pragma once
#include "../vector_matrix/VectorMatrix.hpp"
#include <vector>
#include <utility>
#include <limits>
#include <cmath>

namespace LinearAlgebra{
    /**
     * \brief LU decomposition with partial pivoting
     * 
     * Decompose matrix A such that:
     * PA = LU
     * 
     * Where:
     *  P is permutation matrix
     *  L is lower triangular with unit diagonal
     *  U is upper triangular
     */ 
    template<typename T>
    class DecomposeLU {
    private:
        VectorMatrix<T> matrix;
        double detP;
        void elemination(int col);
        void initL() {
            int rows = matrix.getRows();
            int cols = matrix.getColumns();
            L = std::vector<std::vector<T>>(rows, std::vector<T>(cols, 0));
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    L[i][j] = (i == j) ? 1 : 0;
                }
                
            }
            
        };
        void initP() {
            int n = matrix.getRows();
            P = std::vector<int>(n);
            for(int i=0; i < n; i++){
                P[i] = i; 
            }
        };
        int pivoting(int col);
        std::vector<std::vector<T>> L;
        std::vector<std::vector<double>> U;
        std::vector<int> P; //vector of swap
        static constexpr T eps = std::numeric_limits<T>::epsilon() * static_cast<T>(100);
        void decomposition();
    public:
       using FloatType = std::conditional_t<std::is_same_v<T, float>, float, double>;
        DecomposeLU(VectorMatrix<T> m):matrix(m) { 
            detP = 1;
            decomposition(); 
        }
        double det() const ;
        VectorMatrix<T> inv();
        const std::vector<std::vector<FloatType>>& getU() const{ return U; }
        const std::vector<std::vector<FloatType>>& getL() const{ return L; }
        const std::vector<int>& getP() const{ return P; }
    };
}


template<typename T>
int LinearAlgebra::DecomposeLU<T>::pivoting(int col) {
    T pivotVal = std::abs(U[col][col]);
    int pivot = col;
    int rows = matrix.getRows();
    if (col < 0 || col >= matrix.getRows()) {
        throw std::runtime_error("Invalid column index");
    }
    for (int i = col+1; i < rows; i++) {
        if(std::abs(U[i][col]) > pivotVal) {
            pivotVal = std::abs(U[i][col]);
            pivot = i;
        }
    }
    if (pivotVal <= eps) throw std::runtime_error("Matrix is singular");
    return pivot;
}


/**
 * 
 */
template<typename T>
void LinearAlgebra::DecomposeLU<T>::elemination(int col) {
    int rows = matrix.getRows();
    int cols = matrix.getColumns();
    const T eps = std::numeric_limits<T>::epsilon() * 100;
    if(std::abs(U[col][col]) <= eps) throw std::runtime_error("Pivot is zero");
    for (int i = col+1; i < rows; i++)
    {
        L[i][col]=U[i][col]/U[col][col];
        for (int j = col+1; j < cols; j++)
        {
            U[i][j] = U[i][j] - L[i][col] * U[col][j];
        }
        U[i][col] = 0;
    }
    
}


template<typename T>
void LinearAlgebra::DecomposeLU<T>::decomposition() {
    if (matrix.getRows() != matrix.getColumns())
     throw std::runtime_error("Matrix must be square for LU decomposition");
    int cols = matrix.getColumns();
    int rows = matrix.getRows();
    U = matrix.getMatrix();
    initL();
    initP();
    int swapCount = 0;
    for (int k = 0; k < cols; ++k)
    {
        int pivot = pivoting(k);
        if(pivot != k) {
            using std::swap;
            swap(U[pivot],U[k]);
            swap(P[pivot],P[k]);
            for(int j = 0; j < k; ++j)
                swap(L[k][j],L[pivot][j]);
            swapCount++;
        }
        elemination(k);
    }
    detP = swapCount % 2 == 0 ? 1 : -1;
}

/**
 * \brief Determinant matrix A
 * Computed as:
 *   det(A) = det(P) * product(diag(U))
 */
template<typename T>
double LinearAlgebra::DecomposeLU<T>::det() const{
    auto U = getU();
    int rows = U.size();
    double det = 1;
    for (int i = 0; i < rows; i++) det*=static_cast<double>(U[i][i]);
    
    return det*detP;
}