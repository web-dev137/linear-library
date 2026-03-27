#pragma once
#include "MatrixOperation.h"

template <typename T>
class MultiplyOperation : public MatrixOperation<T> {

public:
   MultiplyOperation() = default;
   Matrix<T> execute(const Matrix<T>& a, const Matrix<T>* b) override {
        return a * *b;
   } 

  const char* getSymbol() const override{
      return "*";
   }

   bool isBinary() const {
      return true;
   }

   bool isUnary() const {
      return false;
   }

   bool isScalar() const {
      return false;
   }
};