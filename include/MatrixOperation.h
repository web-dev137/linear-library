#pragma once
#include "libs/matrix/Matrix.hpp"


template <typename T>
class MatrixOperation
{
public:

   virtual Matrix<T> execute(const Matrix<T>& a, const Matrix<T>* b) = 0;
   virtual ~MatrixOperation() = default;
   virtual const char* getSymbol() const = 0;
   virtual bool isBinary() const = 0;
   virtual bool isScalar() const = 0;
   virtual bool isUnary() const = 0;
};
