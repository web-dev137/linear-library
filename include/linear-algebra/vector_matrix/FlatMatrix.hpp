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
    public:
        FlatMatrix(const std::vector<std::vector<T>>& v) {
            if (v.empty() || v[0].empty()) {
                throw std::invalid_argument("Matrix is empty");
            }

            validation(v);

            rows = v.size();
            cols = v[0].size();
            flatMatrix.reserve(static_cast<size_t>(rows) * cols);
            for (auto& row : flatMatrix) {
                flatMatrix.insert(flatMatrix.end(),
                    flatMatrix.begin(),
                    flatMatrix.end()
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
            cols = (rows>0)?v.begin()->size() : 0;

            flatMatrix.reserve(rows*cols);

            for (const auto& row : v) {
                if(row.size( ! cols)) {
                    throw std::invalid_argument("All rows must have the same size");
                }
                flatMatrix.insert(flatMatrix.end(),row.begin(),row.end());
            }
        }

        T operator()(int i, int j) {
            if((i < 0 || i > rows) || (j < 0 || j > cols)) {
                throw std::out_of_range("Matrix index out of range");
            }

            return flatMatrix[i * cols + j];
        }
        
    };
}