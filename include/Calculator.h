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
    Calculator(/* args */);
    void setOperation(std::unique_ptr<MatrixOperation<T>> op);
    virtual Matrix<T> calc(const Matrix<T>& a, const Matrix<T>* b) = 0;
    virtual ~Calculator() = default;
};