import numpy as np
from io import StringIO


def align_number(number: float) -> str:
    if number == 0:
        return '        0.000'
    number = float(number)
    output = StringIO()
    if number < 0:
        output.write(' ' * (8 - len(str(int(-number)))))  # Выравнивание для отрицательных чисел
        output.write(f"{number:7.3f}".replace(' ', ''))  # Форматированный вывод числа
    else:
        output.write(' ' * (9 - len(str(int(number)))))  # Выравнивание для положительных чисел
        output.write(f"{number:7.3f}".replace(' ', ''))  # Форматированный вывод числа
    output = output.getvalue()
    return output


class LinearModel:
    slots = 'a b c x minmax optimalValue transform'.split()

    def __init__(self, a=np.empty([0, 0]), b=np.empty([0, 0]), c=np.empty([0, 0]), minmax="MAX"):
        self.a = a
        self.b = b
        self.c = c
        self.x = np.array([float(0)] * len(c))  # Инициализация массива x нулевыми значениями
        self.minmax = minmax
        self.optimalValue = None
        self.transform = False

    def printInfo(self):
        print("Коэффициенты =", ', '.join((f'{i:.1f}' for i in self.x)))  # Вывод коэффициентов из массива x
        print(f"Оптимальное значение = {self.optimalValue:.3f}", )  # Вывод оптимального значения

    def optimize(self):
        num_var = len(self.c)  # Количество переменных
        num_slack = len(self.a)  # Количество искусственных переменных

        t1 = np.hstack(([None], [0], self.c, [0] * num_slack))  # Первая строка таблицы
        basis = np.array([0] * num_slack)  # Базисные переменные

        for i in range(0, len(basis)):
            basis[i] = num_var + i  # Назначение базисным переменным номеров столбцов

        a = self.a
        if not ((num_slack + num_var) == len(self.a[0])):
            a = np.hstack((self.a, np.identity(num_slack)))  # Добавление единичной матрицы для искусственных переменных

        t2 = np.hstack((np.transpose([basis]), np.transpose([self.b]), a))  # Вторая строка таблицы
        table = np.vstack((t1, t2))  # Таблица симплекс-метода
        table = np.array(table, dtype='float')

        print("  Стартовая таблица:")
        self.print_table(table)  # Вывод стартовой таблицы
        print()

        optimal = False
        iteration = 0

        while True:
            print(f"Итерация {iteration}".center(60))
            self.print_table(table)  # Вывод текущей таблицы

            for profit in table[0, 2:]:
                if profit > 0:
                    optimal = False
                    break
                optimal = True

            if optimal:
                break

            n = table[0, 2:].tolist().index(np.amax(table[0, 2:])) + 2  # Номер разрешающего столбца
            minimum = np.inf
            row = -1

            for i in range(1, len(table)):
                if table[i, n] > 0:
                    val = table[i, 1] / table[i, n]
                    if val < minimum:
                        minimum = val
                        row = i

            pivot = table[row, n]  # Разрешающий элемент

            print()
            print("  Разрешающая колонна =", n - 1)
            print("              строка =", row)
            print("              элемент =", pivot)

            table[row, 1:] = table[row, 1:] / pivot
            for i in range(0, len(table)):
                if i != row:
                    mult = table[i, n] / table[row, n]
                    table[i, 1:] = table[i, 1:] - mult * table[row, 1:]

            table[row, 0] = n - 2
            iteration += 1
            print()

        print(f"\n{'#'*60}\n")
        print("Финальная таблица была получена за", iteration + 1, "итерации")

        self.x = np.array([0] * len(self.c), dtype=float)  # Обнуление массива x
        for key in range(1, len(table)):
            if table[key, 0] < len(self.c):
                self.x[int(table[key, 0])] = table[key, 1]  # Присваивание значения переменным из таблицы

        self.optimalValue = -1 * table[0, 1]  # Оптимальное значение функции

    def print_table(self, table):
        print(f"ind{' '*8}A0", end=' '*14)
        for i in range(len(self.c)):
            print(f"x{i+1}", end=' '*11)
        for i in range(4):
            print(f"b{i+1}", end=' '*11)
        print()

        for j in range(len(table)):
            i = range(len(table[0]))
            for i in i:
                if not np.isnan(table[j, i]):
                    if i == 0:
                        print('x' + str(int(table[j, i]) + 1), end=' '*2)
                    else:
                        print(align_number(table[j, i]), end='')
                else:
                    print('F', end=' '*3)
            print()


def main():
    a = np.array([[0.9, 1.2, 1.3],
                  [1.1, 1.5, 1.5],
                  [0.7, 0.9, 0.9],
                  [1.3, 1.1, 1.2]])
    b = np.array([2871, 2613, 3960, 1935])
    c = np.array([70, 55, 35])

    model = LinearModel(a, b, c)  # Создание объекта модели

    print("- A:\n", a)
    print("- А0:\n", b)
    print("- C:\n", c)
    print()
    model.optimize()  # Оптимизация модели
    print()
    model.printInfo()  # Вывод результатов


main()