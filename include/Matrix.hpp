#pragma once

#include<iostream>
#include <algorithm>
#include <stdexcept>

template <typename T>
class Matrix {
private:
    int rows,columns; // Размерность матрицы
    T** matrix; // Матрица
public:
    /**
     * Объявление конструкторов
     */
    Matrix(int rows, int columns);
    Matrix(const Matrix<T>& B);
    Matrix(Matrix<T>&& B) noexcept;
    Matrix(): rows(0),columns(0),matrix(nullptr){};
    ~Matrix();

    T** getMatrix();
    int getRows();
    int getColumns();

    /**
     * Перегрузка операторов 
     */
    Matrix<T> operator*(const Matrix<T>& B) const;

    Matrix<T> operator*(T number) const;

    Matrix<T>& operator=(const Matrix<T>& B);
    Matrix<T>& operator=(Matrix<T>&& B) noexcept;

    T& operator()(int i,int j) {
        if(i < 0 || i >= rows || j < 0 || j >=columns) {
            throw std::out_of_range("Matrix index out of range");
        }
        return matrix[i][j]; 
    }
    const T& operator()(int i,int j) const { 
        if(i < 0 || i >= rows || j < 0 || j >=columns) {
            throw std::out_of_range("Matrix index out of range");
        }
        return matrix[i][j];
    }
    

    Matrix<T> operator!()const;
    template <typename U>
    friend std::istream& operator>>(std::istream& in, Matrix<U>& M);
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& M);
    
};

template <typename T>
T** Matrix<T>::getMatrix() {
    return matrix;
}

template <typename T>
int Matrix<T>::getRows() {
    return rows;
}

template <typename T>
int Matrix<T>::getColumns() {
    return columns;
}


/**
 * Опеределение конструктора
 */
template <typename T>
Matrix<T>::Matrix(int rows,int columns): rows(rows), columns(columns){
    matrix = new T*[rows];

    for (int i = 0; i < rows; i++) {
        matrix[i] = new T[columns];
    }  
}

/**
 * Конструктор копирования
 */
template <typename T>
Matrix<T>::Matrix(const Matrix<T>& B) : rows(B.rows),columns(B.columns) {
    matrix = new T*[rows]; // Выделяем память массива указателей на строки
    for(int i = 0; i < rows; i++) {
        matrix[i] = new T[columns];// Выделяем память под строку 
        for(int j = 0; j < columns; j++) {
            matrix[i][j] = B.matrix[i][j];// Копирование
        }
    }
};

/**
 * Move конструктор, его суть в том он перемещает 
 * ресурсы принадлежащие другой матрице(объекту) в текущую матрицу(объекту)
 * без копирования 
 */
template <typename T>
Matrix<T>::Matrix(Matrix<T>&& B) noexcept 
    :matrix(B.matrix), rows(B.rows), columns(B.columns) {
        B.columns = 0;
        B.rows = 0;
        B.matrix = nullptr;
}

/**
 * Move оператор присваивания
 */
template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& B) noexcept {
    if(this != &B) {
        for (int i = 0; i < rows; i++) delete[] matrix[i];
        delete[] matrix;
        
        matrix = B.matrix;
        rows = B.rows;
        columns = B.columns;

        B.columns = 0;
        B.rows = 0;
        B.matrix = nullptr;
    }
    return *this;
}

/**
 * Перегрузка оператора ввода
 */
template <typename U>
std::istream& operator>>(std::istream& in,Matrix<U>& M) {
    std::cout<<"Заполните матрицу(Enter matrix):"<<std::endl;
    for (int i = 0; i < M.rows; i++){
        for (int j = 0; j < M.columns; j++){
            std::cout<<"["<<i<<"]"<<"["<<j<<"]";
            in>>M(i,j);
        }
    }
    return in;
};

/**
 * Перегрузка операции вывода
 */
template <typename U>
std::ostream& operator<<(std::ostream& os,const Matrix<U>& M) {
    for (int i = 0; i < M.rows; ++i){
        for (int j = 0; j < M.columns; ++j){
            os<<M(i,j)<<" ";
        }
        os<<std::endl;
    }
    return os;
}

/**
 * Перегрузка оператора присваивания
 */
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& B) {
    if(this == &B) return *this;

    /**
     * Освобожение памяти
     */
    for (int i = 0; i < rows; i++) delete[] matrix[i];
    delete[] matrix;

    rows = B.rows;
    columns = B.columns;
    
    matrix = new T*[rows]; // Выделяем память массива указателей на строки
    for(int i = 0; i < rows; i++) {
        matrix[i] = new T[columns];// Выделяем память под строку 
        for(int j = 0; j < columns; j++) {
            matrix[i][j] = B.matrix[i][j];// Копирование
        }
    }
    return *this;
}

/**
 * Перегрузка оператора транспонирования матрицы
 */
template <typename T>
Matrix<T> Matrix<T>::operator!() const {
    Matrix<T> result(columns,rows); 

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            result(j,i) = (*this)(i,j);
        }
    }
    return result;
};

/**
 * Перегрузка оператора умножения матрицы на число
 */
template <typename T>
Matrix<T> Matrix<T>::operator*(T number) const {
    Matrix<T> result(rows,columns);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            result.matrix[i][j] = matrix[i][j] * number;
        }
    }
    return result;
};

/**
 * Деструктор 
 */
template <typename T>
Matrix<T>::~Matrix(){
    for (int i = 0; i < rows; i++){
        delete[] matrix[i];
    }

    delete[] matrix;
}

/**
 * Перегрузка оператора умножения матриц
 */
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& B) const {
    if(columns != B.rows) {
        throw std::invalid_argument("num columns A not equal num rows B");
    }

    Matrix<T> result(rows,B.columns);

    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < B.columns; ++j)
        {
            result(i,j) = 0;
            for (int k = 0; k < columns; ++k)
            {
                result(i,j) += (*this)(i,k) * B(k,j);
            }
        }
        
    }

    return result;
};