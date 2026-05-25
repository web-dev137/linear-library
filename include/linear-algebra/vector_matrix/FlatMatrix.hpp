#pragma once

#include<iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <initializer_list>
#include <stdexcept>

namespace LinearAlgebra {
    template <typename T>
    class FlatMatrix {
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
         * \brief Constructs a FlatMatrix from given dimensions and a flat data vector.
         *
         * Creates a matrix with `r` rows and `c` columns, initialized from a flat vector `m`
         * stored in row-major order.
         *
         * \param r Number of rows (must be > 0).
         * \param c Number of columns (must be > 0).
         * \param m Flat vector containing matrix elements in row-major order.
         *
         * \throws std::invalid_argument if r == 0 or c == 0.
         * \throws std::invalid_argument if m.size() != r * c.
         *
         * \note The vector `m` is moved into the internal storage (no copy is made).
         */
        FlatMatrix(int r,int c,std::vector<T>& m):rows(r),cols(c) {
            if(r == 0 || c == 0) {
                throw std::invalid_argument("rows or cols params is 0");
            }

            int expected = r * c;
            if(m.size() != expected) {
                throw std::invalid_argument("wrong size");
            }

            flatMatrix = std::move(m);
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

        FlatMatrix<T> operator*(const FlatMatrix<T>& B) const;
        FlatMatrix<T> operator+(const FlatMatrix<T>& B) const;
        FlatMatrix<T> operator*(const T scalar) const;
        FlatMatrix<T> operator~() const;


        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, FlatMatrix<U>& m);

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

        for(int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                result.flatMatrix[i*rows + j] = flatMatrix[j*cols + i];
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
    FlatMatrix<T> FlatMatrix<T>::operator*(const T scalar) const {
        FlatMatrix result(rows,cols);
        const int size = rows * cols;
        for (int i = 0; i < size; i++) {
            result.flatMatrix[i] = flatMatrix[i] * scalar;
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
    FlatMatrix<T> FlatMatrix<T>::operator*(const FlatMatrix<T>& B) const {
        if(cols != B.getCols()) {
            throw std::invalid_argument("num columns A not equal num rows B");
        }
        
        int colsB = B.getCols();
        FlatMatrix result(rows,colsB);
        const int bs = 32;

        for (int i = 0; i < rows; i += bs) {
            for (int k = 0; k < cols; k += bs) {
                for (int j = 0; j < colsB; j += bs) {

                    int i_end = std::min(i + bs, rows);
                    int k_end = std::min(k + bs, cols);
                    int j_end = std::min(j + bs, colsB);

                    for (int ii = i; ii < i_end; ++ii) {
                        for (int kk = k; kk < k_end; ++kk) {
                            for (int jj = j; jj < j_end; ++jj) {
                                result.flatMatrix[ii*cols+jj] += 
                                    flatMatrix[ii*cols+kk] * B.flatMatrix[kk*cols+jj];
                            }
                        }
                    }
                }
            }
        }
       return result; 
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, FlatMatrix<U>& m) {
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