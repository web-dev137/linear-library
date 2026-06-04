#pragma once

#include<iostream>
#include <algorithm>
#include <vector>
#include "MatrixBase.hpp"
#include <cassert>
#include <initializer_list>
#include <stdexcept>

namespace LinearAlgebra {
    template <typename T>
    using ColumnVector = std::vector<T>;

    template <typename T>
    class FlatMatrix:public MatrixBase<FlatMatrix<T>,T> {
    private:
        int cols,rows;
        std::vector<T> flatMatrix;
        void validation(std::vector<std::vector<T>> v) {
            for (const auto& row : v)
                if (static_cast<int>(row.size()) != cols) 
                    throw std::invalid_argument("All rows must have the same size");
        }
    public:
        FlatMatrix(const std::vector<std::vector<T>>& v) {
            if (v.empty() || v[0].empty()) {
                throw std::invalid_argument("Matrix is empty");
            }

            rows = v.size();
            cols = v[0].size();
            validation(v);
        
            flatMatrix.reserve(rows * cols);
            for (const auto& row : v) {
                flatMatrix.insert(flatMatrix.end(),
                    row.begin(),
                    row.end()
                );
            }
            
        }

        /**
         * \brief Constructs a matrix from nested initializer lists.
         *
         * All rows must have the same number of elements.
         *
         * Example:
         * \code
         * FlatMatrix<int> m = {
         *     {1, 2, 3},
         *     {4, 5, 6}
         * };
         * \endcode
         *
         * \param v Nested initializer list representing matrix rows.
         * \throws std::invalid_argument If rows have different sizes.
         */
        FlatMatrix(std::initializer_list<std::initializer_list<T>> v){
            rows = v.size();
            cols = rows ? v.begin()->size() : 0;
            if(cols == 0) {
                throw std::invalid_argument("Matrix is empty");
            }
            flatMatrix.clear();
            flatMatrix.reserve(rows * cols);

            for (const auto& row : v) {
                if(row.size()!=cols) {
                    throw std::invalid_argument("All rows must have the same size");
                }
                flatMatrix.insert(flatMatrix.end(),row.begin(),row.end());
            }
        }

 
        FlatMatrix(int r,int c):rows(r),cols(c),flatMatrix(r*c,T{}) {}
        FlatMatrix() = default;

        int getRows() const{
            return rows;
        }

        int getCols() const{
            return cols;
        }

        T& operator()(int i, int j) {
            assert(i >= 0 && i < rows && j >= 0 && j < cols && "FlatMatrix: index out of range");
            return flatMatrix[i * cols + j];
        }

        const T& operator()(int i, int j) const{
            assert(i >= 0 && i < rows && j >= 0 && j < cols && "FlatMatrix: index out of range");
            return flatMatrix[i * cols + j];
        }

        FlatMatrix<T> operator~() const;


        template<typename U>
        friend std::ostream& operator<<(std::ostream& os,const FlatMatrix<U>& m);

    };
    
    /**
     * \brief Returns a transposed copy of the matrix.
     *
     * Implements matrix transposition operator (~).
     * For a matrix A of size r x c, returns a new matrix At of size c x r
     * such that At[i][j] = A[j][i].
     *
     * \code
     * A  = [[1,2,3],        At = [[1,4,7],
     *       [4,5,6],  =>          [2,5,8],
     *       [7,8,9]]              [3,6,9]]
     * \endcode
     *
     * \return Transposed matrix (original matrix is not modified).
     */
    template<typename T>
    FlatMatrix<T> FlatMatrix<T>::operator~() const {
        FlatMatrix result(cols,rows);

        for(int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result(j,i) = (*this)(i, j);
            }
        }

        return result;
    }
    
    /**
     * \brief Scalar multiplication
     * Operation:
     * \f[
     * C = A \cdot a
     * \f]
     * where:
     * - \c A is a matrix of type \c FlatMatrix
     * - \c a is a scalar value of the matrix element type
     * - \c C is the resulting matrix
     * 
     * Example:
     * \code
     * int a = 3;
     * FlatMatrix<int> A = {   
     *      {1,2,3},       
     *      {4,5,6},
     *      {7,8,9}
     * };
     * 
     * auto C = A * a;
     * // Result:
     * // {
     * //     { 3,  6,  9},
     * //     {12, 15, 18},
     * //     {21, 24, 27}
     * // }
     * \endcode
     */
    template<typename T>
    FlatMatrix<T> operator*(const FlatMatrix<T>& m,const T scalar) {
        int rows = m.getRows();
        int cols = m.getCols();
        FlatMatrix<T> result(rows,cols);
        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result(i, j) = m(i, j) * scalar;
            }
        }
        return result;
    }

    /**
     * \brief Matrix multiplication
      * Operation:
     * \f[
     * C = A \cdot B
     * \f]
     * where:
     * - \c A is a matrix of type \c FlatMatrix
     * - \c B is a matrix of type \c FlatMatrix
     * Example:
     * \code
     * int a = 3;
     * FlatMatrix<int> A = {   
     *      {1,2,3},       
     *      {4,5,6},
     *      {7,8,9}
     * };
     * 
     * FlatMatrix<int> B = {
     *      {11,12,13},
     *      {14,15,16},
     *      {17,18,19}
     * };
     * 
     * auto C = A * B;
     * // Result:
     * // {
     * //  {90,96,102},
     * //  {216,231,246},
     * //  {342,366,390}
     * // }
     * \endcode
    */
    template<typename T>
    FlatMatrix<T> operator*(const FlatMatrix<T>& A,const FlatMatrix<T>& B) {
        int rowsA = A.getRows();
        int rowsB = B.getRows();
        int colsA = A.getCols();
        int colsB = B.getCols();
        if(colsA != rowsB) {
            throw std::invalid_argument("num columns A not equal num rows B");
        }

        FlatMatrix<T> result(rowsA,colsB);
        const int bs = 32;

        for (int i = 0; i < rowsA; i += bs) {
            for (int k = 0; k < colsA; k += bs) {
                for (int j = 0; j < colsB; j += bs) {

                    int i_end = std::min(i + bs, rowsA);
                    int k_end = std::min(k + bs, colsA);
                    int j_end = std::min(j + bs, colsB);

                    for (int ii = i; ii < i_end; ++ii) {
                        for (int kk = k; kk < k_end; ++kk) {
                            for (int jj = j; jj < j_end; ++jj) {
                                result(ii,jj) += A(ii,kk) * B(kk,jj);
                            }
                        }
                    }
                }
            }
        }
       return result; 
    }

    template<typename T>
    ColumnVector<T> operator*(const FlatMatrix<T>& m, const ColumnVector<T>& v) {
        if(v.size() != m.getCols()) {
            throw std::invalid_argument("vector must be same as columns in matrix");
        }

        ColumnVector<T> result(m.getRows());
        for(int i = 0; i < m.getRows(); i++) {
            for (int j = 0; j < m.getCols(); j++)
            {
                result[i] += m(i,j) * v[j];
            }
        }
        return result;
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, const FlatMatrix<U>& m) {
        int rows = m.rows;
        int cols = m.cols;
        for(int i =0; i < rows; ++i) {
            for(int j = 0; j < cols; ++j) {
                os<<m(i,j);
                if (j + 1 < cols)
                os << ' ';
            }
            if (i + 1 < rows)
            os << '\n';
        }
        return os;
    }



}