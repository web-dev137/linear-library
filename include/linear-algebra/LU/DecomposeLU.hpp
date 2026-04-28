#pragma once
#include "../vector_matrix/VectorMatrix.hpp"
#include <vector>
#include <utility>
#include <limits>
//#include <type_traits>
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
        static_assert(std::is_floating_point_v<T>, 
          "T must be a floating-point type (float or double)");
    private:
        
        VectorMatrix<T> matrix;
        double detP;
        void elemination(int col);
        void initL() {
            int rows = matrix.getRows();
            int cols = matrix.getColumns();
            L = std::vector<std::vector<T>>(rows, std::vector<T>(cols, 0));
            for (int i = 0; i < rows; i++) {
                L[i][i] = 1;  
            }
            
        };
        void initP() {
            int n = matrix.getRows();
            P = std::vector<int>(n);
            for(int i=0; i < n; i++){
                P[i] = i; 
            }
        };
        std::vector<int> initInvP() const {
            int n = matrix.getRows();
            std::vector<int>invP(n,-1);
            for (int i = 0; i < n; i++)
            {
                int pi = P[i];
                if (pi < 0 || pi >= n) throw std::invalid_argument("invalid permutation");
                invP[pi] = i;
            }
            return invP;
        };
        int pivoting(int col);
        std::vector<std::vector<T>> L;
        std::vector<std::vector<T>> U;
        std::vector<int> P; //vector of swap
        static constexpr T eps = std::numeric_limits<T>::epsilon() * static_cast<T>(100);
        void decomposition();
    public:

        DecomposeLU(const VectorMatrix<T>& m) : matrix(m) { 
            detP = 1;
            decomposition(); 
        }

        DecomposeLU(VectorMatrix<T>&& m) : matrix(std::move(m)) { 
            detP = 1;
            decomposition(); 
        }
        double det() const;
        VectorMatrix<T> inv() const;
        const std::vector<std::vector<T>>& getU() const{ return U; }
        const std::vector<std::vector<T>>& getL() const{ return L; }
        const std::vector<int>& getP() const{ return P; }
    };


    template<typename T>
    int DecomposeLU<T>::pivoting(int col) {
        T pivotVal = std::abs(U[col][col]);
        int pivot = col;
        int rows = matrix.getRows();
        if (col < 0 || col >= rows) {
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
    void DecomposeLU<T>::elemination(int col) {
        int rows = matrix.getRows();
        int cols = matrix.getColumns();
        
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
    void DecomposeLU<T>::decomposition() {
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
            elemination(k);
            int pivot = pivoting(k);
            if(pivot != k) {
                using std::swap;
                swap(U[pivot],U[k]);
                swap(P[pivot],P[k]);
                swapCount++;
            }
        }
        detP = swapCount % 2 == 0 ? 1 : -1;
    }

    /**
     * \brief Determinant matrix A
     * Computed as:
     *   det(A) = det(P) * product(diag(U))
     */
    template<typename T>
    double DecomposeLU<T>::det() const{
        auto U = getU();
        int rows = U.size();
        double det = 1;
        for (int i = 0; i < rows; i++) det*=U[i][i];
        
        return det*detP;
    }

    template<typename T>
    VectorMatrix<T> DecomposeLU<T>::inv() const{
        int rows = matrix.getRows();
        int cols = matrix.getColumns();
        std::vector<std::vector<T>> X(rows, std::vector<T>(cols));
        std::vector<int> invP = initInvP();
        std::vector<double> b(rows), y(rows), x(rows);
        for(int i = 0; i < rows; ++i) {
            std::fill(b.begin(), b.end(), 0.0);
            std::fill(y.begin(), y.end(), 0.0);
            std::fill(x.begin(), x.end(), 0.0);
            b[invP[i]] = 1.0;
            
            for (int j = 0; j < cols; ++j) {
                    double sum = 0;
                    for (int k = 0; k < j; ++k) sum += L[j][k] * y[k];
                    y[j] = (b[j] - sum)/L[j][j];
            }

            for (int k = rows-1; k >= 0; --k){
                    double sum = 0;
                    for (int s = k+1; s < cols; ++s) sum += U[k][s] * x[s];
                    x[k] = (y[k] - sum)/U[k][k];
            }

            for (int k = 0; k < rows; ++k) X[k][i] = std::round(x[k]*100)/100;
        }


        return VectorMatrix<T>(std::move(X));
    }
}