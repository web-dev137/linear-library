#include "libs/matrix/Matrix.hpp"
#include <iomanip>

template<typename T>
struct BinaryExpr
{
    const Matrix<T>& A;
    const Matrix<T>& B;
    const Matrix<T>& C;
    const char* op;
};

template<typename T>
struct UnaryExpr
{
    const Matrix<T>& A;
    const Matrix<T>& C;
    const char* op;
};

template<typename T>
struct ScalarExpr
{
    const Matrix<T>& A;
    T scalar;
    const Matrix<T>& C;
    const char* op;
};
 
template<typename T>
std::ostream& operator<<(std::ostream& os, const BinaryExpr<T>& expr) {
    int rows = expr.A.getRows();
    int colsA = expr.A.getColumns();
    int colsB = expr.B.getColumns();
    int colsC = expr.C.getColumns();
    for (int i = 0; i < rows; ++i) {
        for(int j = 0; j < colsA; ++j) {
            os<< std::setw(2) << expr.A(i,j) <<" ";
        }

        if(i == rows/2) os<< *expr.op;
            else os<<" ";

        for (int j = 0; j < colsB; ++j)
        {
            os<< std::setw(2) << expr.B(i,j) <<" ";
        }

        os<<(i == rows/2? "= " :"  ");

        for(int j = 0; j < colsC; ++j) {
            os<< std::setw(2)<< expr.C(i,j)<<" ";
        }
        os<<"\n";
    }

    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const UnaryExpr<T>& expr) {
    int rows = expr.A.getRows();
    int colsA = expr.A.getColumns();
    int colsC = expr.C.getColumns();
    for (int i = 0; i < rows; ++i) {
          if(i == rows/2) os<< *expr.op;
            else os<<" ";

        for(int j = 0; j < colsA; ++j) {
            os<< std::setw(2) << expr.A(i,j) <<" ";
        }

        os<<(i == rows/2? "= " :"  ");

        for(int j = 0; j < colsC; ++j) {
            os<< std::setw(2)<< expr.C(i,j)<<" ";
        }
        os<<"\n";
    }

    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const ScalarExpr<T>& expr) {
    int rows = expr.A.getRows();
    int colsA = expr.A.getColumns();
    int colsC = expr.C.getColumns();

    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < colsA; j++)
        {
            os<<expr.A(i,j)<<" ";
        }
        
        if(i == rows/2) {
            os<<*expr.op;
            os<<" "<<expr.scalar;
            os<<" = ";
        } else { os<<std::setw(7);}

        
        
        for(int j = 0; j < colsC; ++j) {
            os<<expr.C(i,j)<<" ";
        }

        os<<"\n";
    }

    return os;
}

 
