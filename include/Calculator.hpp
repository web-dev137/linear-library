#pragma once
#include <functional>
#include <memory>
#include "Matrix.hpp"
#include "MatrixOperation.h"

template <typename T>
class Calculator
{
private:
    std::unique_ptr<MatrixOperation<T>> operation;
public:
    Calculator(/* args */) = default;
    void setOperation(std::unique_ptr<MatrixOperation<T>> op);
    Matrix<T> calc(const Matrix<T>& a, const Matrix<T>* b);
    virtual ~Calculator() = default;
    bool isBinary() const;
};

template <typename T>
void Calculator<T>::setOperation(std::unique_ptr<MatrixOperation<T>> op) {
    operation = std::move(op);
}

template <typename T>
Matrix<T> Calculator<T>::calc(const Matrix<T>& a, const Matrix<T>* b) {
if(!operation) 
    throw std::runtime_error("Operaton not set");

    return operation->execute(a, b);
}

template <typename T>
bool Calculator<T>::isBinary()const {
    return operation->isBinary();
}