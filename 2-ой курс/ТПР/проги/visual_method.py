import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import time

def f(x1, x2):
    return 2 * x1 + 2 * x2

gradient_vector = np.array((1, 2))
gradient_vector[0], gradient_vector[1] = 2, 2 # df/dx1, df/dx2

antigradient_vector = -gradient_vector

def f_grad(x):
    return - 5 - gradient_vector[1]/gradient_vector[0] * x

def first(x):
    return (12 - x) / 3

def second(x):
    return 34 - 7 * x

fig, ax = plt.subplots()

x1 = np.linspace(-10, 10, 1000)
x2_1 = first(x1)
x2_2 = second(x1)
x_axis = [np.zeros(len(x1)), x1]
y_axis = [x1, np.zeros(len(x1))]
y = np.minimum(x2_1, x2_2)
where = [x1[i] >= 0 and y[i] >= 0 for i in range(len(x1))]
mult = 1

gradient = f_grad(x1)
gradient_line, = ax.plot(x1, gradient)
gradient_line.set_label('gradient f(x) = 2 * x1 + 2 * x2')

ax.plot(x1, x2_1, color='red', label='x1 + 3 * x2 = 12')
ax.plot(x1, x2_2, color='blue', label='7 * x1 + x2 = 34')
ax.plot(*x_axis, *y_axis, color='black')
ax.fill_between(x1, y, color='gray', alpha=0.3, where=where)

def update(frame):
    base_y = gradient_line.get_ydata().copy()

    if True in (base_y <= y):
        base_y +=  gradient_vector[1] * (0.05)

    gradient_line.set_ydata(base_y)

    return gradient_line,

anim = FuncAnimation(fig, update, interval=30)

ax.set_xlabel('x1')
ax.set_ylabel('x2')

ax.set_ylim(-2, 6)
ax.set_xlim(-2, 6)

ax.legend()

plt.show()