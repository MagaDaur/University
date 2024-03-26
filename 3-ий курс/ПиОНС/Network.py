import numpy as np
import pandas as pd
import math
import sys
import random
from matplotlib import pyplot as plt
from tqdm import tqdm

def out(net):
    return 1 / (1 + math.exp(-net))

class Network:
    def __init__(self, info : list[int]):
        if len(info) < 2:
            raise "Not enough data"

        self.learn_rate = 0.5

        self.layers = len(info)
        self.num_inputs = info[0]
        self.num_outputs = info[-1]

        self.neurons = []
        self.biases = []
        self.weights = []
        self.deltas = []

        for layer in info:
            self.neurons.append([ 0 for _ in range(layer) ].copy())
            self.biases.append([ -1 for _ in range(layer) ].copy())
            self.deltas.append([ 0 for _ in range(layer) ].copy())

        for i in range(self.layers - 1):
            self.weights.append(np.random.rand(info[i], info[i+1]) * 2. - 1.)
    
    def feed(self, x):
        self.neurons[0] = x.copy()

    def forward(self):
        for i in range(self.layers - 1):
            neuron_row = self.neurons[i]
            weight_row = self.weights[i]

            self.neurons[i + 1] = neuron_row @ weight_row
            for j in range(len(self.neurons[i + 1])):
                self.neurons[i + 1][j] = out(self.neurons[i + 1][j] + self.biases[i + 1][j])

    def output(self):
        return self.neurons[-1]

    def backward(self, ans):
        self.forward()

        for i in range(self.num_outputs):
            self.deltas[-1][i] = self.neurons[-1][i] * (1 - self.neurons[-1][i]) * (ans[i] - self.neurons[-1][i])

        for i in range(self.layers - 2, -1, -1):
            for j in range(len(self.neurons[i])):
                for k in range(len(self.neurons[i + 1])):
                    self.deltas[i][j] += self.weights[i][j][k] * self.deltas[i + 1][k]
                self.deltas[i][j] *= self.neurons[i][j] * (1 - self.neurons[i][j])

        for layer in range(self.layers - 1, 0, -1):
            for i in range(len(self.neurons[layer])):
                self.biases[layer][i] -= self.learn_rate * self.deltas[layer][i]
                for j in range(len(self.neurons[layer - 1])):
                    self.weights[layer - 1][j][i] += self.learn_rate * self.deltas[layer][i] * self.neurons[layer - 1][j]

train_data = pd.DataFrame(open(sys.path[0] + "/mnist_train.csv"))
test_data = pd.DataFrame(open(sys.path[0] + "/mnist_test.csv"))

network = Network([28 * 28, 16, 16, 10])

for i in tqdm(range(1, len(train_data.index))):
    train = train_data.iloc[i][0][:-1].split(',')
    train[0] = int(float(train[0]))

    for j in range(1, len(train)):
        train[j] = float(train[j]) / 255

    network.feed(train[1:])

    network.backward([1. if idx == train[0] else 0. for idx in range(10)])

# for i in range(len(network.weights)):
#     np.savetxt(f'weights_{i}.csv', network.weights[i])

# test_idxs = list(range(1, len(test_data.index)))
# random.shuffle(test_idxs)

# for i in test_idxs:
#     test = test_data.iloc[i][0][:-1].split(',')
#     test[0] = int(float(test[0]))

#     inputs = []
#     image = []

#     for j in range(1, len(test)):
#         image.append(int(test[j]))

#     image = np.array(image).reshape((28, 28))

#     for j in range(1, len(test)):
#         inputs.append(float(test[j]) / 255)

#     network.feed(inputs)

#     network.forward()
#     output = network.output()

#     print(f'{test[0]}: I thinks it\'s {np.argmax(output)} by {output[np.argmax(output)] * 100:.2f}%')

#     plt.imshow(image, cmap='gray')
#     plt.show()

