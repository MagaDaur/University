import pandas as pd
import sys
import numpy as np
import math

def out(net):
    return 1 / (1 + math.exp(-net))

class Neuron:
    def __init__(self):
        self.bias = np.random.random() * 2. - 1.
        self.value = 0

pixels_frame = pd.DataFrame(open(sys.path[0] + "/train.csv"))
result_frame = pd.DataFrame(open(sys.path[0] + "/ans.csv"))

pixels = [ float(s) / 255. for s in pixels_frame[0][1][:-1].split(',') ]
answers = [int(float(s[:-1])) for s in result_frame[0][1:]]

output = np.zeros(10)
output[answers[0]] = 1.

w1 = np.random.rand(28 * 28, 16) - 0.5
w2 = np.random.rand(16, 16)
w3 = np.random.rand(16, 10)

n1 = [Neuron() for _ in range(16)]
n2 = [Neuron() for _ in range(16)]
n3 = [Neuron() for _ in range(10)]

W = [w1, w2, w3]
N = [n1, n2, n3]

for _ in range(1000):

    c1 = pixels @ w1
    for i in range(16):
        c1[i] -= n1[i].bias
        n1[i].value = out(c1[i])
        c1[i] = n1[i].value

    c2 = c1 @ w2
    for i in range(16):
        c2[i] -= n2[i].bias
        n2[i].value = out(c2[i])
        c2[i] = n2[i].value

    c3 = c2 @ w3
    for i in range(10):
        c3[i] -= n3[i].bias
        n3[i].value = out(c3[i])
        c3[i] = n3[i].value

    n_row = N[2]
    for i in range(len(n_row)):
        neuron = n_row[i]

        out_error = neuron.value * (1 - neuron.value) * (output[i] - neuron.value)

        w_row = W[2]
        for j in range(len(w_row)):
            in_error = N[2 - 1][j].value * (1 - N[2 - 1][j].value) * (w_row[j][i] * out_error)

            w_row[j][i] += 0.5 * out_error * N[2 - 1][j].value

error = 0.5 * sum([(output[i] - c3[i]) ** 2 for i in range(10)])
print(error)

print(W[1])
