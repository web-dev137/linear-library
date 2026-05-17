#pragma once

#include<iostream>
#include <algorithm>
#include <vector>
#include <initializer_list>
#include <stdexcept>

namespace LinearAlgebra {
    /**
     * \brief
     * Examples:
     * \code
     *      VectorMatrix<int> A({
     *          {1,2,3},
     *          {4,5,6},
     *          {7,8,9}
     *      });
     * 
     *      VectorMatrix<int> B({
     *          {11,12,13},
     *          {14,15,16},
     *          {17,18,19}
     *      });
     *      
     *      auto C = A*3;
     *      auto D = A*B;
     *      auto E = !A
     *      std::cout<<A;
     * \endcode
     */
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

        FlatMatrix(int rows,int cols,std::vector<T>& m) {
            if(rows == 0 || cols == 0) {
                throw std::invalid_argument("rows or cols params is 0");
            }

            size_t expected = rows * cols;
            if(m.size() != expected) {
                throw std::invalid_argument("wrong size");
            }

            flatMatrix = std::move(m);
        }

        FlatMatrix(std::initializer_list<std::initializer_list<T>> v){
            rows = v.size();
            cols = v.begin()->size();

            flatMatrix.clear();
            flatMatrix.reserve(rows * cols);

            for (const auto& row : v) {
                if(row.size()!=cols) {
                    throw std::invalid_argument("All rows must have the same size");
                }
                flatMatrix.insert(flatMatrix.end(),row.begin(),row.end());
            }
        }

 
        FlatMatrix(int i,int j):flatMatrix(rows*cols,T(0)),rows(i),cols(j) {}
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
    
    template<typename T>
    FlatMatrix<T> FlatMatrix<T>::operator~() const {
        FlatMatrix result(cols,rows);

        for(int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                result.flatMatrix[j*cols + i] = flatMatrix[i*cols +j];
            }
        }

        return result;
    }
    
    template<typename T>
    FlatMatrix<T> FlatMatrix<T>::operator*(const T scalar) const {
        FlatMatrix result(rows,cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.flatMatrix[i] = flatMatrix[i] * scalar;
            }
        }
        return result;
    }

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
            for(int j = 0; j < rows; ++j) {
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