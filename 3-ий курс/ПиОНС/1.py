import numpy as np
import sys
import matplotlib.pyplot as plt

# Точки (x, y) для обучения и начальные веса w
inp = [[0.5, 2.3], [3.5, 0.8], [4.8, 2.9]]
w = [0, 0, 0]

# Функция РБФ
def h(x, n):
    return np.exp(((x - n) ** 2) / (-2)) #h1(x)=exp(-((x-a1)^2)/2)


# Функция для предсказания значений на основе весов и РБФ
def f(w, n, x):
    return np.dot(w, [h(x, n_i) for n_i in n])


# Функция для вычисления центров интервалов по x
def center(k = inp):
    return [(k[j][0] + k[j + 1][0]) / 2 for j in range(len(k) - 1)]


# Вычисление матрицы u и её транспонирование
a = center()
u = np.asarray([[h(inp[j][0], a[i]) for j in range(len(inp))] for i in range(len(a))])
j = u.dot(u.transpose())

# Вычисление обратной матрицы
rev = np.linalg.inv(j)

# Умножение обратной матрицы на матрицу u
summ = rev.dot(u)

# Формирование вектора Y из исходных значений y
Y = np.asarray([inp[i][1] for i in range(len(inp))])

# Умножение на вектор Y
summ = summ.dot(Y)

# Формирование векторов x, y и yb
x = [inp[k][0] for k in range(len(inp))]
y = [f(summ, a, inp[k][0]) for k in range(len(inp))]
yb = [inp[k][1] for k in range(len(inp))]
print(x, y, yb)

plt.plot(x, y, label='Predicted')
plt.plot(x, yb, label='Actual')
plt.title('Optimization function')
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
plt.grid(True)
plt.show()

'''
Веса между входным и скрытым слоями обучаются таким образом, чтобы приблизить входные данные к целевым значениям. 

1. Определяются центры и отклонения для радиальных элементов;
2. Оптимизируются параметры линейного выходного слоя.

Расположение центров должно соответствовать кластерам, реально
присутствующим в исходных данных. 

После того, как определено расположение центров, нужно найти отклонения.
Величина отклонения определяет «остроту» гауссовой функции. 
'''
