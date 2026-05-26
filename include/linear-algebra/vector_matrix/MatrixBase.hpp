#pragma once

template <typename MatrixType, typename T>
class MatrixBase {
public:
    int getRows() const{ return matrix().getRows(); }

    int getCols() const{ return matrix().getCols(); }

    T& operator()(int i, int j) { return matrix()(i,j); }
    const T& operator()(int i, int j) const{ return matrix()(i,j); }
private:
    const MatrixType& matrix() const { return static_cast<const MatrixType&>(*this); };
    MatrixType& matrix() { return static_cast<MatrixType&>(*this); };
};