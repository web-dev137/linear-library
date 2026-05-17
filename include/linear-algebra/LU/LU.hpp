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
        double detP;
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
            detP = 1;
            decomposition(); 
        }

        LU(FlatMatrix<T>&& m) : matrix(std::move(m)) { 
            detP = 1;
            decomposition(); 
        }
        T det() const;
        FlatMatrix<T> inv() const;
        const std::vector<int>& getP() const{ return P; }
        FlatMatrix<T> getMatrix() const{ return matrix;}
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
        for (int k = 0; k < n; ++k) {
            int pivot = pivoting(k);
            if(pivot != k) {
                for (int j = 0; j < n; j++) {
                    std::swap(matrix(k,j), matrix(pivot,j));
                }
                
                std::swap(P[k], P[pivot]);
            }
            elimination(k);
        }
        
    }
}