#pragma once
#include "../vector_matrix/FlatMatrix.hpp"
#include <vector>
#include <utility>
#include <limits>
#include <type_traits>
#include <cmath>
#include <numeric>

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
    class LU {
        static_assert(std::is_floating_point_v<T>, 
          "T must be a floating-point type (float or double)");
    private:
        
        FlatMatrix<T> matrix;
        int signP = 1;
        void elimination(int col);
        void initP() {
            P.resize(matrix.getRows());
            std::iota(P.begin(), P.end(), 0);
        };
        std::vector<int> initInvP() const {
            int n = matrix.getRows();
            std::vector<int>invP(n,-1);
            for (int i = 0; i < n; i++) {
                int pi = P[i];
                if (pi < 0 || pi >= n) throw std::invalid_argument("invalid permutation");
                invP[pi] = i;
            }
            return invP;
        };
        int pivoting(int col);
        void forwardSubstitution(std::vector<T>& y, const std::vector<T>& b, int n) const;
        void backwardSubstitution(std::vector<T>& x, const std::vector<T>& y, int n) const;
        std::vector<int> P; //vector of swap
        static constexpr T eps = std::numeric_limits<T>::epsilon() * static_cast<T>(100);
        void decomposition();
    public:

        LU(const FlatMatrix<T>& m) : matrix(m) { 
            decomposition(); 
        }

        LU(FlatMatrix<T>&& m) : matrix(std::move(m)) { 
            decomposition(); 
        }
        T det() const;
        FlatMatrix<T> inv() const;
        const std::vector<int>& getP() const{ return P; }
        const FlatMatrix<T>& getMatrix() const{ return matrix;}
    };


    template<typename T>
    int LU<T>::pivoting(int col) {
        T pivotVal = std::abs(matrix(col,col));
        int pivot = col;
        int n = matrix.getRows();
        for (int i = col+1; i < n; i++) { 
            T val = std::abs(matrix(i,col));
            if(val > pivotVal) {
                pivotVal = val;
                pivot = i;
            }
        }
        return pivot;
    }

    template<typename T>
    void LU<T>::elimination(int col) {
        T pivot = matrix(col,col);
        int n = matrix.getRows();
        for (int i = col+1; i < n; i++)
        {
            T factor = matrix(i,col)/pivot;
            matrix(i,col) = factor;
            for (int j = col+1; j < n; j++)
            { 
                matrix(i,j) -= factor * matrix(col,j); 
            }
        }
        
    }

    template<typename T>
    void LU<T>::decomposition() {
        if (matrix.getRows() != matrix.getCols())
        throw std::runtime_error("Matrix must be square for LU decomposition");
        int n = matrix.getRows();
        initP();
        int swapCount = 0;
        for (int k = 0; k < n; ++k) {
            int pivot = pivoting(k);
            if(pivot != k) {
                for (int j = 0; j < n; j++) {
                    std::swap(matrix(k,j), matrix(pivot,j));
                }
                
                std::swap(P[k], P[pivot]);
                swapCount++;
            }
            if (std::abs(matrix(k,k)) <= eps) {
                throw std::runtime_error("Matrix is singular or nearly singular at pivot " + std::to_string(k));
            }
            elimination(k);
        }
        signP = swapCount % 2 == 0 ? 1 : -1;
    }

    template<typename T>
    T LU<T>::det() const {
        int n = matrix.getRows();
        T res = T(1);
        for(int i = 0; i < n; i++) {
            res *= matrix(i,i);
        }

        return res*T(signP);
    }

    template<typename T>
    void LU<T>::forwardSubstitution(std::vector<T>& y, const std::vector<T>& b, int n) const {
        y[0] = b[0];
        for (int i = 1; i < n; ++i) {
            T sum = 0;
            for (int j = 0; j < i; ++j) {
                sum += matrix(i,j) * y[j];
            }
            y[i] = (b[i] - sum);
        }
    }

    template<typename T>
    void LU<T>::backwardSubstitution(std::vector<T>& x, const std::vector<T>& y, int n) const {
        for (int i = n-1; i >= 0; --i){
            T sum = 0;
            for (int j = i+1; j < n; ++j) {
                sum += matrix(i,j) * x[j];
            }
            x[i] = (y[i] - sum)/matrix(i,i);
        }
    }

    template<typename T>
    FlatMatrix<T> LU<T>::inv() const {
        int n = matrix.getRows();
        FlatMatrix<T> X(n,n);
        std::vector<int> invP = initInvP();
        std::vector<T> b(n), y(n), x(n);
        int prev = invP[0];
        b[prev] = T(1);

        for(int i = 0; i < n; ++i) {
            int curr = invP[i];

            if (i > 0) {
                b[prev] = T(0);
                b[curr] = T(1);
            }

            forwardSubstitution(y, b, n);
            backwardSubstitution(x, y, n);

            for (int k = 0; k < n; ++k) {
                X(k,i) = x[k];
            }

            prev = curr;
        }
        return X;
    }
}