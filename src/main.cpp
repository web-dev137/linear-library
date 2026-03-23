#include "Matrix.hpp"
#include <functional>
#include <unordered_map>
#include <windows.h>
#include <iostream>

template <typename T>
using Operation = std::function<void(Matrix<T>&)>;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::unordered_map<int, Operation<int>> ops;

    // Регистрация команд 
    ops[1] = [](Matrix<int>& A) {
        int rows, cols;
        std::cout << "Введите размерность матрицы B: ";
        std::cin >> rows >> cols;

        Matrix<int> B(rows, cols);
        std::cout << "Введите данные в матрицу B: ";
        std::cin >> B;

        Matrix<int> C = A * B;

        std::cout << "Результат:\n" << C;
    };

    ops[2] = [](Matrix<int>& A) {
        int number;
        std::cout << "Введите число: ";
        std::cin >> number;

        Matrix<int> C = A * number;
        std::cout << "Результат:\n" << C;
    };

    ops[3] = [](Matrix<int>& A) {
        Matrix<int> C = !A;
        std::cout << "Результат:\n" << C;
    };

    while (true) {
        std::cout << "\n1) Умножение A * B\n";
        std::cout << "2) Умножение A * number\n";
        std::cout << "3) Транспонирование\n";
        std::cout << "0) Выход\n";

        std::cout<<"Выберите пункт в меню:"<<std::endl;
        int choice;
        std::cin >> choice;

        if (choice == 0) break;

        if (ops.find(choice) == ops.end()) {
            std::cout << "Неверный выбор\n";
            continue;
        }

        int rows, cols;
        std::cout << "Введите размерность матрицы A: ";
        std::cin >> rows >> cols;

        Matrix<int> A(rows, cols);
        std::cout << "Введите данные в матрицу A: ";
        std::cin >> A;

        ops[choice](A);
    }

    return 0;
}