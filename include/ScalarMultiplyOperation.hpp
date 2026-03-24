#pragma once
#include "MatrixOperation.h"
#include "Matrix.hpp"

template <typename T>
class ScalarMultiplyOperation : public MatrixOperation<T>
{
private:
    T scalar;
public:    
    explicit ScalarMultiplyOperation(T s):scalar(s){};
    Matrix<T> execute(const Matrix<T>& a, const Matrix<T>* b) override {
        return a * scalar;
    }
    bool isBinary() const override{ return false;}
};