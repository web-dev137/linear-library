#pragma once
#include "../vector_matrix/VectorMatrix.hpp"
#include <vector>
#include <utility>
#include <limits>
#include <type_traits>
#include <cmath>

namespace LinearAlgebra{
    /**
     * \brief LU decomposition with partial pivoting
     * 
     * Decompose matrix A such that:
     * PA = LU
     * 
     * Where:
     *  P is permutation matrix
     *  L is lower triangular with unit diagonal
     *  U is upper triangular
     */ 
    template<typename T>
    class DecomposeLU {
        static_assert(std::is_floating_point_v<T>, 
          "T must be a floating-point type (float or double)");
    private:
        
        VectorMatrix<T> matrix;
        double detP;
        void elemination(int col);
        void initL() {
            int rows = matrix.getRows();
            int cols = matrix.getColumns();
            L = std::vector<std::vector<T>>(rows, std::vector<T>(cols, 0));
            for (int i = 0; i < rows; i++) {
                L[i][i] = 1;  
            }
            
        };
        void initP() {
            int n = matrix.getRows();
            P = std::vector<int>(n);
            for(int i=0; i < n; i++){
                P[i] = i; 
            }
        };
        std::vector<int> initInvP() const {
            int n = matrix.getRows();
            std::vector<int>invP(n,-1);
            for (int i = 0; i < n; i++)
            {
                int pi = P[i];
                if (pi < 0 || pi >= n) throw std::invalid_argument("invalid permutation");
                invP[pi] = i;
            }
            return invP;
        };
        int pivoting(int col);
        std::vector<std::vector<T>> L;
        std::vector<std::vector<T>> U;
        std::vector<int> P; //vector of swap
        static constexpr T eps = std::numeric_limits<T>::epsilon() * static_cast<T>(100);
        void decomposition();
    public:

        DecomposeLU(const VectorMatrix<T>& m) : matrix(m) { 
            detP = 1;
            decomposition(); 
        }

        DecomposeLU(VectorMatrix<T>&& m) : matrix(std::move(m)) { 
            detP = 1;
            decomposition(); 
        }
        T det() const;
        VectorMatrix<T> inv() const;
        const std::vector<std::vector<T>>& getU() const{ return U; }
        const std::vector<std::vector<T>>& getL() const{ return L; }
        const std::vector<int>& getP() const{ return P; }
    };


    template<typename T>
    int DecomposeLU<T>::pivoting(int col) {
        T pivotVal = std::abs(U[col][col]);
        int pivot = col;
        int rows = matrix.getRows();
        if (col < 0 || col >= rows) {
            throw std::runtime_error("Invalid column index");
        }
        for (int i = col+1; i < rows; i++) {
            auto &Ui = U[i];
            if(std::abs(Ui[col]) > pivotVal) {
                pivotVal = std::abs(Ui[col]);
                pivot = i;
            }
        }
        if (pivotVal <= eps) throw std::runtime_error("Matrix is singular");
        return pivot;
    }


    /**
     * 
     */
    template<typename T>
    void DecomposeLU<T>::elemination(int col) {
        int rows = matrix.getRows();
        int cols = matrix.getColumns();
        
        if(std::abs(U[col][col]) <= eps) throw std::runtime_error("Pivot is zero");
        T pivot = U[col][col];
        for (int i = col+1; i < rows; i++)
        {
            auto &Ui = U[i];
            T& Li_col = L[i][col];
            Li_col=Ui[col]/pivot;
            const auto &Ucol = U[col]; 

            for (int j = col+1; j < cols; j++)
            {
                Ui[j] -= Li_col * Ucol[j];
            }
            Ui[col] = static_cast<T>(0);
        }
        
    }


    template<typename T>
    void DecomposeLU<T>::decomposition() {
        if (matrix.getRows() != matrix.getColumns())
        throw std::runtime_error("Matrix must be square for LU decomposition");
        int cols = matrix.getColumns();
        int rows = matrix.getRows();
        U = matrix.getMatrix();
        initL();
        initP();
        int swapCount = 0;
        for (int k = 0; k < cols; ++k)
        {
            int pivot = pivoting(k);
         
            if(pivot != k) {
                using std::swap;
                swap(U[pivot],U[k]);
                swap(P[pivot],P[k]);
                if (k > 0)
                swap_ranges(L[pivot].begin(), L[pivot].begin() + k, L[k].begin());
                swapCount++;
            }
            elemination(k);
        }
        detP = swapCount % 2 == 0 ? 1 : -1;
    }

    /**
     * \brief Determinant matrix A
     * Computed as:
     *   det(A) = det(P) * product(diag(U))
     */
    template<typename T>
    T DecomposeLU<T>::det() const{
        const auto& U = getU();   
        int rows = U.size();
        T det = T(1);
        for (int i = 0; i < rows; i++) det*=U[i][i];
        
        return det*detP;
    }

    template<typename T>
    VectorMatrix<T> DecomposeLU<T>::inv() const{
        int rows = matrix.getRows();
        int cols = matrix.getColumns();
        std::vector<std::vector<T>> X(rows, std::vector<T>(cols));
        std::vector<int> invP = initInvP();
        std::vector<T> b(rows), y(rows), x(rows);

        for(int i = 0; i < rows; ++i) {
            b.assign(rows,0);
            b[invP[i]] = T(1);
            
            for (int j = 0; j < cols; ++j) {
                    T sum = 0;
                    auto& Lj = L[j];
                    auto& Ljj = L[j][j];
                    for (int k = 0; k < j; ++k) sum += Lj[k] * y[k];
                    y[j] = (b[j] - sum)/Ljj;
            }

            for (int k = rows-1; k >= 0; --k){
                    T sum = 0;
                    auto& Uk = U[k];
                    auto& Ukk = U[k][k];
                    for (int s = k+1; s < cols; ++s) sum += Uk[s] * x[s];
                    x[k] = (y[k] - sum)/Ukk;
            }

            for (int k = 0; k < rows; ++k) {
                auto& Xki = X[k][i];
                Xki = x[k];
            }
        }


        return VectorMatrix<T>(std::move(X));
    }
}