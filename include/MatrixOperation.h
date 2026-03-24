#pragma once
#include "Matrix.hpp"

template <typename T>
class MatrixOperation
{
public:
   virtual Matrix<T> execute(const Matrix<T>& a, const Matrix<T>* b) = 0;
   virtual ~MatrixOperation() = default;
   virtual bool isBinary() const {return true;}
};
