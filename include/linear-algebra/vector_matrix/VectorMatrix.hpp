#pragma once

#include<iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>

namespace LinearAlgebra{
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
    class VectorMatrix {
    private:
        int cols,rows;
        std::vector<std::vector<T>> matrix; // Матрица
    public:
        VectorMatrix(int rows, int cols):rows(rows), cols(cols){
            matrix.assign(rows,std::vector<T>(cols,0));
        };
        VectorMatrix(const std::vector<std::vector<T>>& v):matrix(v) {
            if (v.empty() || v[0].empty()) {
                throw std::invalid_argument("Matrix is empty");
            }
            rows = v.size();
            cols = matrix[0].size();

            validation();
        }
          VectorMatrix(std::vector<std::vector<T>>&& v)
      : matrix(std::move(v)) {
            if (matrix.empty() || matrix[0].empty()) throw std::invalid_argument("Matrix is empty");
            rows = static_cast<int>(matrix.size());
            cols = static_cast<int>(matrix[0].size());
            validation();
        }
        void validation() {
            for (const auto& row : matrix)
                if (static_cast<int>(row.size()) != cols) 
                    throw std::invalid_argument("All rows must have the same size");
        }
        VectorMatrix(): rows(0),cols(0){};
        ~VectorMatrix()=default;

        VectorMatrix<T> operator*(const VectorMatrix<T>& B) const;

        VectorMatrix<T> operator*(T number) const;

        T& operator()(int i,int j) {
            
            if(i < 0 || i >= rows || j < 0 || j >=cols) {
                throw std::out_of_range("Matrix index out of range");
            }

            return matrix[i][j]; 
        }
        const T& operator()(int i,int j) const { 
            if(i < 0 || i >= rows || j < 0 || j >=cols) {
                throw std::out_of_range("Matrix index out of range");
            }
            return matrix[i][j];
        }

        std::vector<T>& operator[](int i) {
        if (i < 0 || i >= rows)
            throw std::out_of_range("Row index out of range");
            return matrix[i];
        }

        /**
         * \brief Return row of matrix
         * \code
         * A = [
         *      [1,2],
         *      [3,4]
         *  ]
         * A[0] return row under index 0:  [1,2]
         * \endcode
         */
        const std::vector<T>& operator[](int i) const {
            if (i < 0 || i >= rows)
                throw std::out_of_range("Row index out of range");
            return matrix[i];
        }

        /// @brief return count rows
        /// @return 
        int getRows() const {return rows;}

        /// @brief return count columns
        int getColumns() const {return cols;}
        
        const std::vector<std::vector<T>>& getMatrix()const {return matrix;}
        
        VectorMatrix<T> operator!()const;
        template <typename U>
        friend std::istream& operator>>(std::istream& in, VectorMatrix<U>& M);
        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const VectorMatrix<U>& M);
        
    };
}

/**
 * Print matrix
 */
template <typename U>
std::ostream& operator<<(std::ostream& os,const LinearAlgebra::VectorMatrix<U>& M) {
    int rows = M.getRows();
    int cols = M.getColumns();
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            os<<M(i,j)<<" ";
        }
        if(i != rows-1)
            os<<"\n";
    }
    return os;
}

/**
 * \brief Transponse matrix
 * \code
 * A = [[1,2,3],        At = [[1,4,7]
 *      [4,5,6],   =>        [2,5,8],
 *      [7,8,9]]             [9,6,3]]
 * \endcode
 */
template <typename T>
LinearAlgebra::VectorMatrix<T> LinearAlgebra::VectorMatrix<T>::operator!() const {
    VectorMatrix<T> result(cols,rows); 
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < rows; j++){
            result.matrix[j][i] = matrix[i][j];
        }
    }
    return result;
};

/**
 * \brief Scalar multiplication
 * A*a
 * where:
 * A is matrix of VectorMatrix type
 * a is number of type that set in template
 */
template <typename T>
LinearAlgebra::VectorMatrix<T> LinearAlgebra::VectorMatrix<T>::operator*(T number) const {

    VectorMatrix<T> result(rows,cols);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            result.matrix[i][j] = matrix[i][j] * number;
        }
    }
    return result;
};



/**
 * \brief Matrix multiplication
 * A*B
 * where:
 * A is matrix of VectorMatrix type
 * B is matrix of VectorMatrix type
 */
template <typename T>
LinearAlgebra::VectorMatrix<T> LinearAlgebra::VectorMatrix<T>::operator*(const VectorMatrix<T>& B) const {
    if(cols != B.getRows()) {
        throw std::invalid_argument("num columns A not equal num rows B");
    }

    int colsB = B.getColumns();
    VectorMatrix<T> result(rows,colsB);

    
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < colsB; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < this->cols; k++)
            {
                result.matrix[i][j] += matrix[i][k] * B.matrix[k][j];
            }
        }
        
    }

    return result;
};