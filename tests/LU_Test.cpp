#include <gtest/gtest.h>
#include "../include/linear-algebra/LU/DecomposeLU.hpp"

using namespace LinearAlgebra;
class LU_Test : public ::testing::Test
{
protected:
    VectorMatrix<double> A;
    std::unique_ptr<DecomposeLU<double>> lu;
    void SetUp() override {
        A = VectorMatrix<double>({
            {2, 5, 7},
            {6, 3, 4},
            {5, -2, -3}
        });
        lu = std::make_unique<DecomposeLU<double>>(A);
    }

    /**
     * Rebuilds A with row permutations (from P) to validate that PA = LU
     */
    VectorMatrix<double> reconstructA() {
 
        auto P = lu->getP();
    
        int rows = A.getRows();
        VectorMatrix<double> PA = A;
        for (int i = 0; i < rows; i++)
            PA[i] = A[P[i]];
        
        return PA;
    }

    template <typename T>
    void compare(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B) {
        for (int i = 0; i < A[0].size(); i++)
        {
            for (int j = 0; j < A.size(); j++)
            {
                EXPECT_DOUBLE_EQ(A[i][j],B[i][j]);
            }    
        }
    }
};

TEST_F(LU_Test,decomposition) {

    VectorMatrix<double> PA = reconstructA();
    auto U = lu->getU();
    auto L = lu->getL();
    VectorMatrix<double> vU(U);
    VectorMatrix<double> vL(L);
    VectorMatrix<double> LU = vL*vU;
    compare<double>(PA.getMatrix(),LU.getMatrix());
    
}

TEST_F(LU_Test, det) {
    double det = lu->det();
    det = std::round(det*100)/100;
    EXPECT_DOUBLE_EQ(-1.0,det);
}

TEST_F(LU_Test, inv) {
    auto D = lu->inv();
    auto data = std::vector<std::vector<double>>{
        {1.0, -1.0, 1.0},
        {-38.0, 41.0, -34.0},
        {27.0, -29.0, 24.0}
    };
    auto E = VectorMatrix<double>(std::move(data));

    compare<double>(D.getMatrix(),E.getMatrix());
}