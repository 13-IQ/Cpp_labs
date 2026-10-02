import time
import sys

def calculate_expression(x):
    return x**2 - x**2 + x * 4 - x * 5 + x + x

def main():
    while True:
        try:
            user_input = input("Введите количество итераций (или не-число для выхода): ")
            n = int(user_input)
            
            if n <= 0:
                print("Количество итераций должно быть положительным.\n")
                continue
            
            start_time = time.perf_counter()
            
            for i in range(n):
                x = float(i) 
                result = calculate_expression(x)
            
            end_time = time.perf_counter()

            duration_ms = (end_time - start_time) * 1000
            
            print(f"Выполнено {n} итераций за {duration_ms:.3f} мс\n")
            
        except ValueError:
            print("Введено не число. Завершение программы.")
            break

if __name__ == "__main__":
    main()
