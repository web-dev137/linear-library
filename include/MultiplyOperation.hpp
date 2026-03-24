#pragma once
#include "MatrixOperation.h"

template <typename T>
class MultiplyOperation : public MatrixOperation<T> {
public:
   MultiplyOperation() = default;
   Matrix<T> execute(const Matrix<T>& a, const Matrix<T>* b) override {
        return a * *b;
   } 
   bool isBinary() const override{ return true;}
};