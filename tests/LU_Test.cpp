#include <gtest/gtest.h>
#include "../include/libs/LU/LU.hpp"

class LU_Test : public ::testing::Test
{
protected:
    LU<int> lu;
    VectorMatrix<int> A;
    void SetUp() override {
        A = VectorMatrix<int>({
            {1,2,3},
            {4,5,6},
            {7,8,9}
        });
        lu = LU<int>(A);
    }
};
