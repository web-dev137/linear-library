#include <gtest/gtest.h>
#include "../include/linear-algebra/vector_matrix/FlatMatrix.hpp"

using namespace LinearAlgebra;

class FlatMatrixTest : public ::testing::Test
{
protected:
   
};

TEST(FlatMatrixTest,operatorGet) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    FlatMatrix<int> A(2,3,v);
    double val = A(0,2);
    EXPECT_DOUBLE_EQ(val,3.0);
}