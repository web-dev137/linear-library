#pragma once
#include "LU.hpp"
#include <vector>

namespace LinearAlgebra{
    template<typename T, typename MatrixType>
    class Inversion {
    public:
        Inversion(const LU<T,MatrixType>& lu)
            :lu(lu),
            matrix(lu.getMatrix()),
            P(lu.getP()),
            n(lu.getMatrix().getRows()){};

        MatrixType inv() const;
    private:
        const LU<T,MatrixType>& lu;
        const MatrixType& matrix;
        std::vector<int> P;
        int n;
        std::vector<int> initInvP() const {
            std::vector<int>invP(n,-1);
            for (int i = 0; i < n; i++) {
                int pi = P[i];
                if (pi < 0 || pi >= n) throw std::invalid_argument("invalid permutation");
                invP[pi] = i;
            }
            return invP;
        };
        void forwardSubstitution(std::vector<T>& y, const std::vector<T>& b, int n) const;
        void backwardSubstitution(std::vector<T>& x, const std::vector<T>& y, int n) const;
    };

    template<typename T, typename MatrixType>
    void Inversion<T, MatrixType>::forwardSubstitution(std::vector<T>& y, const std::vector<T>& b, int n) const {
        y[0] = b[0];
        for (int i = 1; i < n; ++i) {
            T sum = 0;
            for (int j = 0; j < i; ++j) {
                sum += matrix(i,j) * y[j];
            }
            y[i] = (b[i] - sum);
        }
    }

    template<typename T, typename MatrixType>
    void Inversion<T, MatrixType>::backwardSubstitution(std::vector<T>& x, const std::vector<T>& y, int n) const {
        for (int i = n-1; i >= 0; --i){
            T sum = 0;
            for (int j = i+1; j < n; ++j) {
                sum += matrix(i,j) * x[j];
            }
            x[i] = (y[i] - sum)/matrix(i,i);
        }
    }

    /**
     * \brief Returns the inverse matrix.
     *
     * \return The inverse matrix computed from the LU decomposition.
     */
    template<typename T, typename MatrixType>
    MatrixType Inversion<T, MatrixType>::inv() const {
        MatrixType X(n,n);
        std::vector<int> invP = initInvP();
        std::vector<T> b(n), y(n), x(n);
        int prev = invP[0];
        b[prev] = T(1);

        for(int i = 0; i < n; ++i) {
            int curr = invP[i];

            if (i > 0) {
                b[prev] = T(0);
                b[curr] = T(1);
            }

            forwardSubstitution(y, b, n);
            backwardSubstitution(x, y, n);

            for (int k = 0; k < n; ++k) {
                X(k,i) = x[k];
            }

            prev = curr;
        }
        return X;
    }
}