import numpy as np
import pandas as pd
import math
import sys
import cv2

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
        error = [sum([(self.neurons[-1][i] - ans[i]) ** 2 for i in range(self.num_outputs)]) / self.num_outputs]

        while len(error) < 100 and error[-1] > 0.01:
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

            self.forward()
            error.append(sum([(self.neurons[-1][i] - ans[i]) ** 2 for i in range(self.num_outputs)]) / self.num_outputs)

        print(f'Trained {ans}, error = {error[-1]}')
        return error


train_data = pd.read_csv(sys.path[0] + "/train.csv")

network = Network([28 * 28, 16, 16, 1])

for i in range(1, len(train_data.index)):
    train = train_data.iloc[i][0][:-1].split(',')
    train[0] = int(float(train[0]))

    for j in range(1, len(train)):
        train[j] = float(train[j]) / 255

    network.feed(train[1:])

    network.backward([1. if idx == train[0] else 0. for idx in range(10)])
