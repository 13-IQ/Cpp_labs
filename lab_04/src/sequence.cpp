#include <iostream>
#include <chrono>

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
    std::cout << "--- Тест на " << iterations << " итераций ---" << std::endl;

    auto start_total = std::chrono::high_resolution_clock::now();

    double res1 = formula1(x, iterations);
    double res2 = formula2(x, iterations);
    double res3 = formula3(res1, res2);

    auto end_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_total = end_total - start_total;

    std::cout << "Результат Формулы 3: " << res3 << std::endl;
    std::cout << "Общее время выполнения: " << duration_total.count() << " сек." << std::endl;
    std::cout << std::endl;
}

int main() {
    double x = 5.0;

    runTest(10000, x);
    runTest(1000000, x);

    return 0;
}
