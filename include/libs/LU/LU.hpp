#include "../vector_matrix/VectorMatrix.hpp"
#include <vector>

template<typename T>
class LU {
private:
    VectorMatrix<T> matrix;
    void decomposition();
    void elemination(int col);
    void initL();
    void pivoting(int col);
    std::vector<std::vector<T>> L;
    std::vector<std::vector<T>> U;
    std::vector<int> P; //vector of swap
public:
    LU(VectorMatrix<T> m):matrix(m) {}
    double det() const ;
    VectorMatrix<T> inv();
    
};