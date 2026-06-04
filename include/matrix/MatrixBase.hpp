#pragma once
#include <type_traits>
template <typename MatrixType, typename T>
class MatrixBase {
    static_assert(std::is_arithmetic_v<T>, 
          "T must be a numeric");
public:
    int getRows() const{ return matrix().getRows(); }

    int getCols() const{ return matrix().getCols(); }

    T& operator()(int i, int j) { return matrix()(i,j); }
    const T& operator()(int i, int j) const{ return matrix()(i,j); }
private:
    const MatrixType& matrix() const { return static_cast<const MatrixType&>(*this); };
    MatrixType& matrix() { return static_cast<MatrixType&>(*this); };
};