#include "Matrix.hpp"
#include <functional>
#include "MatrixOperation.h"
#include "MultiplyOperation.hpp"
#include "TransponseOperation.hpp"
#include "ScalarMultiplyOperation.hpp"
#include <unordered_map>
#include <windows.h>
#include <iostream>
#include <memory>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Matrix<double> C;

    using factory = std::function<std::unique_ptr<MatrixOperation<double>>()>;
    std::unordered_map<int, factory> ops;

    // Регистрация команд 
    ops[1] = [](){ return std::make_unique<MultiplyOperation<double>>();};

    ops[2] = []() {
        double number;
        std::cout << "Введите число: ";
        std::cin >> number;
        return std::make_unique<ScalarMultiplyOperation<double>>(number);
    };

    ops[3] = []() { return std::make_unique<TransponseOperation<double>>();};

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

        auto op = ops[choice]();

        int rows, cols;
        std::cout << "Введите размерность матрицы A: <rows> <cols>";
        std::cin >> rows >> cols;
        Matrix<double> A(rows, cols);
        std::cin >> A;
        if(op->isBinary()) {
            std::cout << "Введите размерность матрицы B: <rows> <cols>";
            std::cin >> rows >> cols;
            Matrix<double> B(rows, cols);
            std::cin >> B;
            C = op->execute(A, &B);
        } else {
            C = op->execute(A,nullptr);
        }
        std::cout<<"Результат:"<<std::endl;
        std::cout<<C;
        
    }

    return 0;
}