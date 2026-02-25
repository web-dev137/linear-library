#include <gtest/gtest.h>
#include "../include/linear-algebra/vector_matrix/VectorMatrix.hpp"

class VectorMatrixTest : public ::testing::Test
{
protected:
    VectorMatrix<double> m1;
    VectorMatrix<double> m2;

    void SetUp() override{
        m1=VectorMatrix<double>({
            {1,2,3},
            {4,5,6},
            {7,8,9}
        });
        m2 = VectorMatrix<double>({
            {11,12,13},
            {14,15,16},
            {17,18,19}
        });
    }

};

TEST_F(VectorMatrixTest,MatrixCorrect) {
    EXPECT_EQ(m1.getColumns(),3);
    EXPECT_EQ(m1.getRows(),3);
};

TEST_F(VectorMatrixTest,MultiplyVectorMatrix) {
    VectorMatrix<double> res = m1*m2;
    VectorMatrix<double> expected({
        {90,96,102},
        {216,231,246},
        {342,366,390}
    });
    EXPECT_EQ(res.getMatrix(),expected.getMatrix());
};

TEST_F(VectorMatrixTest,MultiplyOnScalar) {
    VectorMatrix<double> res = m1 * 1.5;
    VectorMatrix<double> expected({
        {1.5, 3, 4.5},
        {6, 7.5, 9},
        {10.5, 12, 13.5}
    });
    EXPECT_EQ(res.getMatrix(),expected.getMatrix());
}

TEST_F(VectorMatrixTest, Transponse) {
    VectorMatrix<double> expected({
       { 1, 4, 7 },
       { 2, 5, 8 },
       { 3, 6, 9 }
    });

    VectorMatrix<double> res = !m1;

    EXPECT_EQ(res.getMatrix(),expected.getMatrix());
}