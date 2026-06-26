import time
import sys

def calculate_expression(x):
    """Функция для вычисления выражения: x^2 - x^2 + x*4 - x*5 + x + x"""
    return x**2 - x**2 + x * 4 - x * 5 + x + x

def main():
    # Дополнительный цикл на повторный запуск
    while True:
        try:
            user_input = input("Введите количество итераций (или не-число для выхода): ")
            n = int(user_input)
            
            if n <= 0:
                print("Количество итераций должно быть положительным.\n")
                continue
            
            # Засекаем время начала
            start_time = time.perf_counter()
            
            # Выполняем вычисления n раз
            for i in range(n):
                x = float(i)  # x = номер итерации
                result = calculate_expression(x)
                # Раскомментируй, если хочешь видеть результаты:
                # print(f"Итерация {i}: x={x}, результат={result}")
            
            # Засекаем время окончания
            end_time = time.perf_counter()
            
            # Вычисляем затраченное время
            duration_ms = (end_time - start_time) * 1000
            
            print(f"Выполнено {n} итераций за {duration_ms:.3f} мс\n")
            
        except ValueError:
            print("Введено не число. Завершение программы.")
            break

if __name__ == "__main__":
    main()
