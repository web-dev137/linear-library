#include "../vector_matrix/VectorMatrix.hpp"
#include <vector>
#include <utility>
#include <cmath>

template<typename T>
class LU {
private:
    VectorMatrix<T> matrix;
    void elemination(int col);
    void initL() {
        int rows = matrix.getRows();
        int cols = matrix.getColumns();
        L = std::vector<std::vector<T>>(rows, std::vector<T>(cols, 0));
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                L[i][j] = (i == j) ? 1 : 0;
            }
            
        }
        
    };
    void initP() {
        int n = matrix.getRows();
        P = std::vector<T>(n);
        for(int i; i < n; i++){ P[i] = i; }
    };
    int pivoting(int col);
    std::vector<std::vector<T>> L;
    std::vector<std::vector<double>> U;
    std::vector<T> P; //vector of swap
public:
    LU(VectorMatrix<T> m):matrix(m) {}
    void decomposition();
    double det() const ;
    VectorMatrix<T> inv();
    std::vector<std::vector<double>> getU() { return U; }
};



template<typename T>
int LU<T>::pivoting(int col) {
    T pivotVal = abs(U[col][col]);
    int pivot = col;
    int rows = matrix.getRows();
    if (col < 0 || col >= matrix.getRows()) {
        throw std::runtime_error("Invalid column index");
    }
    for (int i = col+1; i < rows; i++) {
        if(abs(U[i][col]) > pivotVal) {
            pivotVal = abs(U[i][col]);
            pivot = i;
        }
    }
    if (pivotVal == 0.0)
            throw std::runtime_error("Matrix is singular");
    return pivot;
}



template<typename T>
void LU<T>::elemination(int col) {
    int cols = matrix.getColumns();
    for (int i = col+1; i < cols; i++)
    {
        L[i][col]=U[i][col]/U[col][col];
        for (int j = col+1; j < cols; j++)
        {
            U[i][j] -= L[i][col] * U[col][j];
        }
        U[i][col] = 0;
    }
    
}

template<typename T>
void LU<T>::decomposition() {
    int cols = matrix.getColumns();
    int rows = matrix.getRows();
    U = matrix.getMatrix();
    initL();
    initP();
   
    for (int k = 0; k < cols; k++)
    {
        int pivot = pivoting(k);
        using std::swap;
        swap(U[pivot],U[k]);
        swap(P[pivot],P[k]);

        elemination(k);
    }
    
}
