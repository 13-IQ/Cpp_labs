#include <iostream>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>

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
    std::cout << "--- Тест на " << iterations << " итераций (процессы) ---" << std::endl;

    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    auto start_total = std::chrono::high_resolution_clock::now();

    auto start_f1 = std::chrono::high_resolution_clock::now();
    pid_t pid1 = fork();
    if (pid1 == 0) {
        close(pipe1[0]);
        double res = formula1(x, iterations);
        auto end_f1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_f1 = end_f1 - start_f1;
        double time_sec = duration_f1.count();
        write(pipe1[1], &res, sizeof(res));
        write(pipe1[1], &time_sec, sizeof(time_sec));
        close(pipe1[1]);
        _exit(0);
    }

    auto start_f2 = std::chrono::high_resolution_clock::now();
    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(pipe2[0]);
        double res = formula2(x, iterations);
        auto end_f2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_f2 = end_f2 - start_f2;
        double time_sec = duration_f2.count();
        write(pipe2[1], &res, sizeof(res));
        write(pipe2[1], &time_sec, sizeof(time_sec));
        close(pipe2[1]);
        _exit(0);
    }

    close(pipe1[1]);
    close(pipe2[1]);

    double res1 = 0, res2 = 0;
    double time_f1 = 0, time_f2 = 0;
    read(pipe1[0], &res1, sizeof(res1));
    read(pipe1[0], &time_f1, sizeof(time_f1));
    close(pipe1[0]);

    read(pipe2[0], &res2, sizeof(res2));
    read(pipe2[0], &time_f2, sizeof(time_f2));
    close(pipe2[0]);

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    auto start_f3 = std::chrono::high_resolution_clock::now();
    double res3 = formula3(res1, res2);
    auto end_f3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_f3 = end_f3 - start_f3;

    auto end_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_total = end_total - start_total;

    std::cout << "Результат Формулы 1: " << res1 << " (время: " << time_f1 << " сек.)" << std::endl;
    std::cout << "Результат Формулы 2: " << res2 << " (время: " << time_f2 << " сек.)" << std::endl;
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
