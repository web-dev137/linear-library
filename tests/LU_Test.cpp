#include <gtest/gtest.h>
#include "../include/linear-algebra/LU/DecomposeLU.hpp"

class LU_Test : public ::testing::Test
{
protected:
    LinearAlgebra::VectorMatrix<double> A;
    std::unique_ptr<LinearAlgebra::DecomposeLU<double>> lu;
    void SetUp() override {
        A = LinearAlgebra::VectorMatrix<double>({
            {0, 2, 1},
            {1, 1, 0},
            {2, 1, 1}
        });
        lu = std::make_unique<LinearAlgebra::DecomposeLU<double>>(A);
    }

    LinearAlgebra::VectorMatrix<double> reconstructA() {
 
        auto P = lu->getP();
    
        int rows = A.getRows();
        LinearAlgebra::VectorMatrix<double> PA = A;
        for (int i = 0; i < rows; i++)
            PA[i] = A[P[i]];   // обратная перестановка
        
        return PA;
    }
};

TEST_F(LU_Test,decomposition) {

    LinearAlgebra::VectorMatrix<double> PA = reconstructA();
    auto U = lu->getU();
    auto L = lu->getL();
    LinearAlgebra::VectorMatrix<double> vU(U);
    LinearAlgebra::VectorMatrix<double> vL(L);
    LinearAlgebra::VectorMatrix<double> LU = vL*vU;
    for (int i = 0; i < PA.getRows(); i++)
    {
        for (int j = 0; j < PA.getColumns(); j++)
        {
            EXPECT_DOUBLE_EQ(PA[i][j],LU[i][j]);
        }
        
    }
    
}

TEST_F(LU_Test, det) {
    double det = lu->det();

    EXPECT_DOUBLE_EQ(-3,det);
}
