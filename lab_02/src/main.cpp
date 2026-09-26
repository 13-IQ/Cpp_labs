#include <iostream>
#include <chrono>
#include <cmath>

double calculate_expression(double x) {
    return std::pow(x, 2) - std::pow(x, 2) + x * 4 - x * 5 + x + x;
}

int main() {
    int n;
    
    while (true) {
        std::cout << "Введите количество итераций (или не-число для выхода): ";
        
        if (!(std::cin >> n)) {
            std::cout << "Введено не число. Завершение программы.\n";
            break;
        }
        
        if (n <= 0) {
            std::cout << "Количество итераций должно быть положительным.\n";
            continue;
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < n; ++i) {
            double x = static_cast<double>(i); 
            double result = calculate_expression(x);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration = end - start;
        
        std::cout << "Выполнено " << n << " итераций за " << duration.count() << " мс\n\n";
    }
    
    return 0;
}
