#include <gtest/gtest.h>
#include "../include/linear-algebra/LU/LU.hpp"

using namespace LinearAlgebra;
class LU_Inplace_Test : public ::testing::Test
{
protected:
    FlatMatrix<double> A;
    std::unique_ptr<LU<double>> lu;
    void SetUp() override {
         A = {
            {2.0, 5.0, 7.0},
            {6.0, 3.0, 4.0},
            {5.0, -2.0, -3.0}
        };
        lu = std::make_unique<LU<double>>(A);
    }

    template<typename T>
    void compare(FlatMatrix<T> res, FlatMatrix<T> exp) {
        int resRows = res.getRows();
        int resCols = res.getCols();
        int expRows = exp.getRows();
        int expCols = exp.getCols();
        ASSERT_EQ(resRows, expRows);
        ASSERT_EQ(resCols, expCols);

        for(int i = 0; i < resRows; i++) {
            for(int j = 0; j < resRows; j++) {
                EXPECT_NEAR(res(i,j),exp(i,j),1e-9);
            }
        }
    }

    template<typename T>
    FlatMatrix<T> extractL(const FlatMatrix<T>& A) {   
        int n = A.getRows();
        FlatMatrix<T> L(n, n);             

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i > j) {                   
                    L(i, j) = A(i, j);
                } else if (i == j) {
                    L(i, j) = T(1.0);          
                }
            }
        }
        return L;
    }

    template<typename T>
    FlatMatrix<T> extractU(const FlatMatrix<T>& A) {
        int n = A.getRows();
        FlatMatrix<T> U(n, n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i <= j) {              
                    U(i, j) = A(i, j);
                }
            }
        }
        return U;
    }
};

TEST_F(LU_Inplace_Test,decomposition) {
    FlatMatrix<double>exp = {
        {6, 3, 4},
        {5.0/6, -4.5, -19.0/3},
        {1.0/3, -8.0/9, 1.0/27}
    };

    auto P = lu->getP();

    int n = A.getRows();
    FlatMatrix<double> PA = A;
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            PA(i,j) = A(P[i],j);
        }
    }
    auto m = std::move(lu->getMatrix());
    FlatMatrix<double> L = extractL(m);
    FlatMatrix<double> U = extractU(m);
    auto LUproduct = L*U;
    compare(PA,LUproduct);
}

TEST_F(LU_Inplace_Test, det) {
    double det = lu->det();
    EXPECT_NEAR(-1.0, det, 1e-9);
}

TEST_F(LU_Inplace_Test, inv) {
    FlatMatrix<double>exp = {
        {1.0, -1.0, 1.0},
        {-38.0, 41.0, -34.0},
        {27.0, -29.0, 24.0}
    };
    
    auto D = lu->inv();

    compare(D,exp);
}