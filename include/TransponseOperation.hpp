#pragma once
#include "MatrixOperation.h"

template<typename T>
class TransponseOperation : public MatrixOperation<T>{
public:    
    TransponseOperation() = default;
    Matrix<T> execute(const Matrix<T>& a, const Matrix<T>* b) override {
        return !a;
    }

    bool isBinary() const {
        return false;
    }

    bool isUnary() const {
        return true;
    }

    bool isScalar() const {
        return false;
    }

    const char* getSymbol() const override { return "T"; }
};

