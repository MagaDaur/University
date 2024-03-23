import numpy as np

train = [
    [-1, -1],
    [-1, 1],
    [1, -1],
    [1, 1],
]

ans = [
    -1,
    1,
    1,
    1,
]

W = [
    [0],
    [0],
    [0],
]

for i in range(len(train)):
    X = train[i]
    Y = ans[i]

    W[0][0] += X[0] * Y
    W[1][0] += X[1] * Y

    W[2][0] -= Y # T

X = [-1, 1, -1]

M = np.matmul(X, W)

print(1 if M[0] > 0 else -1)