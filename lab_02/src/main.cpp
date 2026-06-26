#include <iostream>
#include <chrono>
#include <cmath>

// Функция для вычисления выражения: x^2 - x^2 + x*4 - x*5 + x + x
double calculate_expression(double x) {
    return std::pow(x, 2) - std::pow(x, 2) + x * 4 - x * 5 + x + x;
}

int main() {
    int n;
    
    // Дополнительный цикл на повторный запуск
    while (true) {
        std::cout << "Введите количество итераций (или не-число для выхода): ";
        
        // Проверяем, что введено число
        if (!(std::cin >> n)) {
            std::cout << "Введено не число. Завершение программы.\n";
            break;
        }
        
        if (n <= 0) {
            std::cout << "Количество итераций должно быть положительным.\n";
            continue;
        }
        
        // Засекаем время начала
        auto start = std::chrono::high_resolution_clock::now();
        
        // Выполняем вычисления n раз
        for (int i = 0; i < n; ++i) {
            double x = static_cast<double>(i); // x = номер итерации
            double result = calculate_expression(x);
            // Раскомментируй, если хочешь видеть результаты:
            // std::cout << "Итерация " << i << ": x=" << x << ", результат=" << result << "\n";
        }
        
        // Засекаем время окончания
        auto end = std::chrono::high_resolution_clock::now();
        
        // Вычисляем затраченное время
        std::chrono::duration<double, std::milli> duration = end - start;
        
        std::cout << "Выполнено " << n << " итераций за " << duration.count() << " мс\n\n";
    }
    
    return 0;
}
