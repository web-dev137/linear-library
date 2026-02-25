#include <gtest/gtest.h>
#include "../include/linear-algebra/LU/LU.hpp"

class LU_Test : public ::testing::Test
{
protected:
    VectorMatrix<double> A;
    std::unique_ptr<LU<double>> lu;
    void SetUp() override {
        A = VectorMatrix<double>({
            {2.0, 1.0, 1.0},
            {4.0, -6.0, 0.0},
            {-2.0, 7.0, 2.0}
        });
        lu = std::make_unique<LU<double>>(A);
    }
};

TEST_F(LU_Test,decomposition) {
    lu->decomposition();
    std::vector<std::vector<double>> E;
    E = std::vector<std::vector<double>>({
        {4.0, -6.0, 0.0},
        {0.0, 4.0, 1.0},
        {0.0, 0.0, 1.0}
    });
    
    auto U = lu->getU();

    for (size_t i = 0; i < E.size(); i++) {
        for (size_t j = 0; j < E[i].size(); j++) {
            EXPECT_DOUBLE_EQ(E[i][j], U[i][j]);
        }
    }
}
