#pragma once
#include <matrix/FlatMatrix.hpp>
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
        void forwardSubstitution(std::vector<T>& y, const std::vector<T>& b, int n) const;
        void forwardSubstitution(std::vector<T>& y, int b_pos, int n) const;
        void backwardSubstitution(std::vector<T>& x, const std::vector<T>& y, int n) const;
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
        MatrixType inv() const;
        ColumnVector<T> solve(const ColumnVector<T>& v) const;
        const std::vector<int>& getP() const{ return P; }
        const MatrixType& getMatrix() const{ return matrix;}
    };


    template<typename T, typename MatrixType>
    int LU<T, MatrixType>::pivoting(int k) {
        T pivotVal = std::abs(matrix(k,k));
        int pivot = k;
        int n = matrix.getRows();
        for (int i = k+1; i < n; i++) { 
            T val = std::abs(matrix(i,k));
            if(val > pivotVal) {
                pivotVal = val;
                pivot = i;
            }
        }
        return pivot;
    }

    template<typename T, typename MatrixType>
    void LU<T, MatrixType>::elimination(int k) {
        T pivot = matrix(k,k);
        int n = matrix.getRows();
        for (int i = k+1; i < n; i++)
        {
            T factor = matrix(i,k)/pivot;
            matrix(i,k) = factor;
            for (int j = k+1; j < n; j++)
            { 
                matrix(i,j) -= factor * matrix(k,j); 
            }
        }
        
    }

    template<typename T, typename MatrixType>
    void LU<T, MatrixType>::decomposition() {
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
                signP = -signP;
            }
            if (std::abs(matrix(k,k)) <= eps) {
                throw std::runtime_error("Matrix is singular or nearly singular at pivot " + std::to_string(k));
            }
            elimination(k);
        }
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

    template<typename T, typename MatrixType>
    void LU<T, MatrixType>::forwardSubstitution(std::vector<T>& y, const std::vector<T>& b, int n) const {
        y[0] = b[0];
        for (int i = 1; i < n; ++i) {
            T sum = 0;
            for (int j = 0; j < i; ++j) {
                sum += matrix(i,j) * y[j];
            }
            y[i] = b[i] - sum;
        }
    }

    template<typename T, typename MatrixType>
    void LU<T, MatrixType>::forwardSubstitution(std::vector<T>& y, int b_pos, int n) const {
        y[0] = b_pos==0;
        for (int i = 1; i < n; ++i) {
            T sum = 0;
            T b = (b_pos == i);
            for (int j = 0; j < i; ++j) {
                sum += matrix(i,j) * y[j];
            }
            y[i] = b - sum;
        }
    }

    template<typename T, typename MatrixType>
    void LU<T, MatrixType>::backwardSubstitution(std::vector<T>& x, const std::vector<T>& y, int n) const {
        for (int i = n-1; i >= 0; --i){
            T sum = 0;
            for (int j = i+1; j < n; ++j) {
                sum += matrix(i,j) * x[j];
            }
            if (std::abs(matrix(i,i)) < std::numeric_limits<T>::epsilon() * 100) {
                throw std::runtime_error("Nearly singular matrix at diagonal " + std::to_string(i));
            }
            x[i] = (y[i] - sum)/matrix(i,i);
        }
    }

    /**
     * \brief Returns the inverse matrix.
     *
     * \return The inverse matrix computed from the LU decomposition.
     */
    template<typename T, typename MatrixType>
    MatrixType LU<T, MatrixType>::inv() const {
        int n = matrix.getRows();
        MatrixType X(n,n);
        std::vector<int> invP = initInvP();
        std::vector<T>  y(n), x(n);
        for(int i = 0; i < n; ++i) {
            int b_pos = invP[i];

            forwardSubstitution(y, b_pos, n);
            backwardSubstitution(x, y, n);

            for (int k = 0; k < n; ++k) {
                X(k,i) = x[k];
            }
        }
        return X;
    }

    template<typename T,typename MatrixType>
    ColumnVector<T> LU<T,MatrixType>::solve(const ColumnVector<T>& v) const {
        if(v.empty()) {
            throw std::invalid_argument("vector is empty");
        }
        ColumnVector<T> pb(v.size());
        for (size_t i = 0; i < v.size(); ++i)
        {
            pb[i] = v[P[i]];
        }
        ColumnVector<T> y(v.size());
        ColumnVector<T> x(v.size());
        forwardSubstitution(y,pb,v.size());
        backwardSubstitution(x,y,v.size());
        return x;
    }

}