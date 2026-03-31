#include "Matrix.hpp"
#include <vector>

template<typename T>
class LU {
private:
    Matrix<T> matrix;
    void decomposition();
    void initL();
    void pivoting();
    std::vector<std::vector<T>> L;
    std::vector<std::vector<T>> U;
public:
    LU(Matrix<T> m):matrix(m) {
        initL();
        decomposition();
    }
    double det();
};

template<typename T>
void LU<T>::initL() {
    int rows = matrix.getRows();
    int cols = matrix.getColumns();
   L(rows,std::vector<T>(cols,0));
   for (int i = 0; i < rows; i++)
   {
        for (int j = 0; j < cols; j++)
        {
            if(i == j) {
                L[i][j] = 1;
            }
        }
        
   }
   
}

template<typename T>
void LU<T>::decomposition() {
    int k = 0;
    int rows = matrix.getRows();
    int cols = matrix.getColumns();
    for (int i = 0; i < count; i++)
    {
        for (int j = k; j < count; j++)
        {
            /* code */
        }
        k++;
    }
    
}