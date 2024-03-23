import numpy as np
import math

def out(net):
    return 1 / (1 + math.exp(-net))

def act(o):
    return 1 if o >= 0.5 else 0

def calc(input, w):
    return out(np.matmul(input + [-1], w)[0])

def train(x_train, y_train, w):
    avg_error = 0
    n = len(x_train)

    for i in range(n):
        x = x_train[i]
        y = y_train[i]

        y_test = out(calc(x, w))

        dy = y - y_test

        err = y_test * (1 - y_test) * dy
        avg_error += dy ** 2

        for j in range(len(x)):
            w[j][0] += err * x[j]
        
        w[-1][0] -= err

    print(avg_error)
    if(avg_error - 1 > 0.1):
        return train(x_train, y_train, w)

    return w

W_AND = [[0],[0],[0]]
W_OR = [[0],[0],[0]]
W_NOT = [[0],[0]]

x_train = [
    [1, 1],
    [0, 0],
    [0, 1],
    [1, 0],
]

W_AND = train(x_train, [1, 0, 0, 0], W_AND)
W_OR = train(x_train, [1, 0, 1, 1], W_OR)
W_NOT = train([[1], [0]], [0, 1], W_NOT)

print(W_AND)
print("AND")
for x in x_train:
    o1 = act(calc(x, W_AND))
    print(x, o1)
print()

print(W_OR)
print("OR")
for x in x_train:
    o1 = act(calc(x, W_OR))
    print(x, o1)
print()

print("NOT")
print(W_NOT, end='\n\n')
print("XOR")
for x in x_train:

    o1 = act(calc(x, W_AND))
    o2 = act(calc([o1], W_NOT))

    o3 = act(calc(x, W_OR))

    o4 = act(calc([o2, o3], W_AND))

    print(x, o4)