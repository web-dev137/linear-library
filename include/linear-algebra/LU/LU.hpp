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
     * \brief LU decomposition with partial pivoting in-place.
    *
    * Decomposes the matrix \f$A\f$ such that
    * \f[
    * P A = L U
    * \f]
    * where \f$P\f$ is a permutation matrix, \f$L\f$ is a unit lower
    * triangular matrix, and \f$U\f$ is an upper triangular matrix.
    *
    * The decomposition is stored in-place.
    */ 
    template<typename T, typename MatrixType>
    class LU {
        static_assert(std::is_floating_point_v<T>, 
          "T must be a floating-point type (float or double)");
    private:
        
        MatrixType matrix;
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
        std::vector<int> P; //vector of swap
        static constexpr T eps = std::numeric_limits<T>::epsilon() * static_cast<T>(100);
        void decomposition();
    public:

        LU(const MatrixType& m) : matrix(m) { 
            decomposition(); 
        }

        LU(MatrixType&& m) : matrix(std::move(m)) { 
            decomposition(); 
        }
        T det() const;
        const std::vector<int>& getP() const{ return P; }
        const MatrixType& getMatrix() const{ return matrix;}
    };


    template<typename T, typename MatrixType>
    int LU<T, MatrixType>::pivoting(int col) {
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

    template<typename T, typename MatrixType>
    void LU<T, MatrixType>::elimination(int col) {
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

    template<typename T, typename MatrixType>
    void LU<T, MatrixType>::decomposition() {
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

    /**
     * \brief Returns the determinant of the matrix.
     *
     * \returns The determinant of the matrix.
     */
    template<typename T, typename MatrixType>
    T LU<T, MatrixType>::det() const {
        int n = matrix.getRows();
        T res = T(1);
        for(int i = 0; i < n; i++) {
            res *= matrix(i,i);
        }

        return res*T(signP);
    }
}