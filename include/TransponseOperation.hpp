#pragma once
#include "MatrixOperation.h"

template<typename T>
class TransponseOperation : public MatrixOperation<T>{
public:    
    TransponseOperation() = default;
    Matrix<T> execute(const Matrix<T>& a, const Matrix<T>* b) override {
        return !a;
    }

    bool isBinary() const override{ return false;}
};

