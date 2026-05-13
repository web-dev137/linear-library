#include <gtest/gtest.h>
#include "../include/linear-algebra/vector_matrix/FlatMatrix.hpp"

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
            {10,11,12},
            {13,14,15},
            {16,17,18}
        };
    }
};

TEST_F(FlatMatrixTest,operatorGet) {
    int val = A(0,2);
    EXPECT_EQ(val,3);
}

TEST_F(FlatMatrixTest, operatorMultiply) {

    FlatMatrix<int> Exp({
        {84,  90,  96},
        {201, 216, 231},
        {318, 342, 366}
    });
    const auto& C = A*B;
    int rows = A.getRows();
    int cols = B.getCols();
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            EXPECT_NEAR(C(i,j),Exp(i,j),1e-9);
        }
    }
}

TEST(FlatMatrixTest,operatorOstream) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    FlatMatrix<int> A(2,3,v);
    std::ostringstream oss;
    oss << A;
    EXPECT_EQ(oss.str(),"1 2 3 \n4 5 6\n");
}