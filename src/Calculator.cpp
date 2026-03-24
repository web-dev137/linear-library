#include "Calculator.h"

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