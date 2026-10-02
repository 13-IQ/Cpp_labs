#include <iostream>
#include <string>
#include <cstdlib>

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double power(double base, double exp) {
    double result = 1.0;
    int n = static_cast<int>(exp);

    if (n < 0) {
        for (int i = 0; i < -n; ++i) {
            result = result * base;
        }
        return 1.0 / result;
    }

    for (int i = 0; i < n; ++i) {
        result = result * base;
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Error! Использование: calc <число1> <число2> <+|-|^>" << std::endl;
        return 1;
    }

    double num1 = std::stod(argv[1]);
    double num2 = std::stod(argv[2]);
    std::string op = argv[3];
    double result = 0;

    if (op == "+") {
        result = add(num1, num2);
    } else if (op == "-") {
        result = subtract(num1, num2);
    } else if (op == "^") {
        result = power(num1, num2);
    } else {
        std::cerr << "Неизвестный оператор. Используйте: + - ^" << std::endl;
        return 1;
    }

    std::cout << "Результат: " << result << std::endl;
    return 0;
}
