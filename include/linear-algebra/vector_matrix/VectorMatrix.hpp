#pragma once

#include<iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>

template <typename T>
class VectorMatrix {
private:
    int cols,rows;
    std::vector<std::vector<T>> matrix; // Матрица
public:
    /**
     * Объявление конструкторов
     */
    VectorMatrix(int rows, int cols):rows(rows), cols(cols){
        matrix.assign(rows,std::vector<T>(cols,0));
    };
    VectorMatrix(const std::vector<std::vector<T>>& v):matrix(v) {
        rows = matrix[0].size();
        cols = v.size();
    }
    VectorMatrix(): rows(0),cols(0){};
    ~VectorMatrix()=default;

    std::vector<std::vector<T>> getMatrix();
    int getRows() const;
    int getColumns() const;

    /**
     * Перегрузка операторов 
     */
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
    

    VectorMatrix<T> operator!()const;
    template <typename U>
    friend std::istream& operator>>(std::istream& in, VectorMatrix<U>& M);
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const VectorMatrix<U>& M);
    
};

template <typename T>
std::vector<std::vector<T>> VectorMatrix<T>::getMatrix() {
    return matrix;
}

template <typename T>
int VectorMatrix<T>::getRows() const {
    return rows; 
}

template <typename T>
int VectorMatrix<T>::getColumns() const {
    return cols;
}


/**
 * Перегрузка операции вывода
 */
template <typename U>
std::ostream& operator<<(std::ostream& os,const VectorMatrix<U>& M) {
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
 * Перегрузка оператора транспонирования матрицы
 */
template <typename T>
VectorMatrix<T> VectorMatrix<T>::operator!() const {
    VectorMatrix<T> result(rows,cols); 

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            result.matrix[j][i] = matrix[i][j];
        }
    }
    return result;
};

/**
 * Перегрузка оператора умножения матрицы на число
 */
template <typename T>
VectorMatrix<T> VectorMatrix<T>::operator*(T number) const {

    VectorMatrix<T> result(rows,cols);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            result.matrix[i][j] = matrix[i][j] * number;
        }
    }
    return result;
};



/**
 * Перегрузка оператора умножения матриц
 */
template <typename T>
VectorMatrix<T> VectorMatrix<T>::operator*(const VectorMatrix<T>& B) const {
    if(cols != B.getRows()) {
        throw std::invalid_argument("num columns A not equal num rows B");
    }

    int cols = B.getColumns();
    VectorMatrix<T> result(rows,cols);

    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j)
        {
            result.matrix[i][j] = 0;
            for (int k = 0; k < cols; ++k)
            {
                result(i,j) += matrix[i][k] * B.matrix[k][j];
            }
        }
        
    }

    return result;
};