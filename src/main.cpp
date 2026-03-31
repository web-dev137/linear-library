#include "libs/matrix/Matrix.hpp"
#include <functional>
#include "MatrixOperation.h"
#include "MultiplyOperation.hpp"
#include "Calculator.hpp"
#include "Formatter.hpp"
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
        Calculator<double> calc;

        calc.setOperation(std::move(op));
        if(calc.getOp()->isBinary()) {
            std::cout << "Введите размерность матрицы B: <rows> <cols>";
            std::cin >> rows >> cols;
            Matrix<double> B(rows, cols);
            std::cin >> B;
            
            C = calc.calc(A,&B);
            BinaryExpr<double> expr = {A,B,C,calc.getOp()->getSymbol()};
            std::cout<<"Результат:"<<std::endl;
            std::cout<<expr;
        } 

        if(calc.getOp()->isUnary()) {
            C = calc.calc(A,nullptr);
            UnaryExpr<double> expr = {A,C,calc.getOp()->getSymbol()};
            std::cout<<"Результат:"<<std::endl;
            std::cout<<expr;
        }

        if(calc.getOp()->isScalar()) {
            C = calc.calc(A,nullptr);
            auto* smOperation = dynamic_cast<ScalarMultiplyOperation<double>*>(calc.getOp());
            ScalarExpr<double> expr = {A,smOperation->getScalar(),C,smOperation->getSymbol()};
            std::cout<<"Результат:"<<std::endl;
            std::cout<<expr;
        }
        
    }

    return 0;
}