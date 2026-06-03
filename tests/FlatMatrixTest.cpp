#include <gtest/gtest.h>
#include <matrix/FlatMatrix.hpp>

using namespace LinearAlgebra;

class FlatMatrixTest : public ::testing::Test
{
protected:
    FlatMatrix<int> A;
    FlatMatrix<int> B;
       void SetUp() override{
        A ={
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };

        B = {
            {11,12,13},
            {14,15,16},
            {17,18,19}
        };
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
};

TEST_F(FlatMatrixTest,operatorGet) {
    int val = A(0,2);
    EXPECT_EQ(val,3);
}

TEST_F(FlatMatrixTest,MultiplyOnScalar) {
    auto res = A * 3;
    FlatMatrix<int> expected = {
        {3, 6, 9},
        {12, 15, 18},
        {21, 24, 27}
    };
    
    compare<int>(res,expected);
}

TEST_F(FlatMatrixTest, operatorTnasponse) {
    FlatMatrix<int> Exp({
        { 1, 4, 7 },
        { 2, 5, 8 },
        { 3, 6, 9 }
    });

    int rows = A.getRows();
    int cols = A.getCols();
    const auto& C = ~A;
    compare<int>(C,Exp);
}

TEST_F(FlatMatrixTest, operatorMultiply) {

    FlatMatrix<int> Exp({
         {90,96,102},
        {216,231,246},
        {342,366,390}
    });
    const auto& C = A*B;
    int rows = A.getRows();
    int cols = B.getCols();
    
    compare<int>(C,Exp);
}

TEST_F(FlatMatrixTest, operatorMultiplyVector) {

    ColumnVector<int> exp = {14, 32, 50};
    ColumnVector<int> v = {1,2,3};
    const auto& res = A*v;
        EXPECT_EQ(exp,res);
}