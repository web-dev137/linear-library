#pragma once
#include "MatrixOperation.h"
#include "libs/matrix/Matrix.hpp"

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

    T getScalar() {
        return scalar;
    }
    
    bool isBinary() const {
        return false;
    }

    bool isUnary() const {
        return false;
    }

    bool isScalar() const {
        return true;
    }
    const char* getSymbol() const override { return "*"; }
};