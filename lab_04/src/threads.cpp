#include <iostream>
#include <chrono>
#include <thread>

double formula1(double x, long long iterations) {
    volatile double localX = x;
    double res = 0;
    for (long long i = 0; i < iterations; ++i) {
        res = localX * localX - localX * localX + localX * 4 - localX * 5 + localX + localX;
    }
    return res;
}

double formula2(double x, long long iterations) {
    volatile double localX = x;
    double res = 0;
    for (long long i = 0; i < iterations; ++i) {
        res = localX + localX;
    }
    return res;
}

double formula3(double res1, double res2) {
    return res1 + res2 - res1;
}

void runTest(long long iterations, double x) {
    std::cout << "--- Тест на " << iterations << " итераций (потоки) ---" << std::endl;

    double res1 = 0, res2 = 0;

    auto start_total = std::chrono::high_resolution_clock::now();

    auto start_f1 = std::chrono::high_resolution_clock::now();
    std::thread t1([&res1, x, iterations]() {
        res1 = formula1(x, iterations);
    });

    auto start_f2 = std::chrono::high_resolution_clock::now();
    std::thread t2([&res2, x, iterations]() {
        res2 = formula2(x, iterations);
    });

    t1.join();
    auto end_f1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_f1 = end_f1 - start_f1;

    t2.join();
    auto end_f2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_f2 = end_f2 - start_f2;

    auto start_f3 = std::chrono::high_resolution_clock::now();
    double res3 = formula3(res1, res2);
    auto end_f3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_f3 = end_f3 - start_f3;

    auto end_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_total = end_total - start_total;

    std::cout << "Результат Формулы 1: " << res1 << " (время: " << duration_f1.count() << " сек.)" << std::endl;
    std::cout << "Результат Формулы 2: " << res2 << " (время: " << duration_f2.count() << " сек.)" << std::endl;
    std::cout << "Результат Формулы 3: " << res3 << " (время: " << duration_f3.count() << " сек.)" << std::endl;
    std::cout << "Общее время выполнения: " << duration_total.count() << " сек." << std::endl;
    std::cout << std::endl;
}

int main() {
    double x = 5.0;

    runTest(10000, x);
    runTest(1000000, x);

    return 0;
}
