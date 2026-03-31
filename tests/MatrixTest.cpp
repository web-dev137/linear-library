#include <gtest/gtest.h>
#include "../include/libs/matrix/Matrix.hpp"

class MatrixTest : public ::testing::Test {
protected:
    Matrix<int> A;
    Matrix<int> B;

    void SetUp() override {
        A = Matrix<int>(3,3);
        B = Matrix<int>(3,3);
        int val = 1;
        for (int i = 0; i < 3; i++)
            for(int j =0; j < 3; j++)
                A(i,j) = val++;

        val = 11;

        for (int i = 0; i < 3; i++){
           for (int j = 0; j < 3; j++){
                B(i,j) = val++;
           }
        }
        
        
    }

    Matrix<int> fromVector(const std::vector<std::vector<int>>& v) {
        Matrix<int> m(v.size(),v[0].size());
        for (int i = 0; i < m.getRows(); i++) {
            for (int j = 0; j < m.getColumns(); j++) {
                m(i,j) = v[i][j];
            }
        }
        return m;
    } 

    void compareMatrix(Matrix<int>& Expect, Matrix<int>& C) {
        for (int i = 0; i < C.getRows(); i++){
            for (int j = 0; j < C.getColumns(); j++){
                EXPECT_EQ(Expect(i,j),C(i,j));
            }
        }
    }

};

TEST_F(MatrixTest,MultiplyMatrix) {
    ASSERT_EQ(A.getRows(),B.getRows());
    ASSERT_EQ(A.getColumns(),B.getColumns());
    Matrix<int> Expect = fromVector({
        {90,96,102},
        {216,231,246},
        {342,366,390}
    });
    
    Matrix<int> C = A * B;
    compareMatrix(Expect,C);
}

TEST_F(MatrixTest,TransponseMatrix) {
    Matrix<int> Expect = fromVector({
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    });
    ASSERT_EQ(Expect.getRows(),A.getRows());
    ASSERT_EQ(Expect.getColumns(),A.getColumns());

    Matrix<int> C = !A;
    compareMatrix(Expect,C);
}

TEST_F(MatrixTest,MultiplyMatrixOnNum) {
    
    Matrix<int> Expect = fromVector({
        {3, 6, 9},
        {12, 15, 18},
        {21, 24, 27}
    });
    ASSERT_EQ(Expect.getRows(),A.getRows());
    ASSERT_EQ(Expect.getColumns(),A.getColumns());
    int num = 3;

    Matrix<int> C = A*num;
    compareMatrix(Expect,C);
}

TEST(CopyConstructorTest,Indepence) {
    Matrix<int> A(2,2);
    A(0,0) = 31;

    Matrix<int> B = A;
    B(0,0) = 74;

    EXPECT_EQ(A(0,0),31);
}

TEST(CopyConstructorTest,DeepCopy) {
    Matrix<int> A(3,3);
    Matrix<int> B = A;

    EXPECT_NE(A.getMatrix(),B.getMatrix());
    for (int i = 0; i < A.getRows(); i++)
    {
        EXPECT_NE(A.getMatrix()[i],B.getMatrix()[i]);
    }
    
}