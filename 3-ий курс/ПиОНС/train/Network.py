import torch
import torch.nn as nn
import torch.nn.functional as F
import pandas as pd
from tqdm import tqdm
import numpy as np
from torch.utils.data import Dataset, DataLoader
import cv2
import math

def dist(lat1, lon1, lat2, lon2):
    earthRadiusKm = 6371000

    dLat = math.radians(lat2-lat1)
    dLon = math.radians(lon2-lon1)

    lat1 = math.radians(lat1)
    lat2 = math.radians(lat2)

    a = math.sin(dLat/2) * math.sin(dLat/2) + math.sin(dLon/2) * math.sin(dLon/2) * math.cos(lat1) * math.cos(lat2); 
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))
    return earthRadiusKm * c


class Net(nn.Module):

    def __init__(self):
        super(Net, self).__init__()

        self.conv1 = nn.Conv2d(1, 1, (2, 1), padding=1)
        self.pool1 = nn.MaxPool2d(2, 2)

        self.conv2 = nn.Conv2d(1, 1, (5, 1), padding=1)
        self.pool2 = nn.MaxPool2d(2, 2)

        self.flat = nn.Flatten()

        self.fc1 = nn.Linear(150 * 230 + 1, 1024)
        self.fc2 = nn.Linear(1024, 100)
        self.fc3 = nn.Linear(100, 1)


    def forward(self, x, distance):
        x = F.relu(self.conv1(x))
        x = self.pool1(x)

        x = F.relu(self.conv2(x))
        x = self.pool2(x)

        x = self.flat(x)

        x = F.relu(self.fc1(x))
        #dropout

        x = F.relu(self.fc2(x))
        #dropout

        x = self.fc3(x)

        return x

model = Net()

criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

class Loader(Dataset):
    def __init__(self, filepath):
        self.data = pd.read_csv(filepath)

    def __len__(self):
        return len(self.data)

    def __getitem__(self, index):
        image = cv2.imread(self.data.iloc[index]['filename'], cv2.IMREAD_GRAYSCALE).astype('float32')

        camera = self.data.iloc[index]['camera'].split(', ')
        camera = [float(coord) for coord in camera]

        building = self.data.iloc[index]['building'].split(', ')
        building = [float(coord) for coord in building]

        distance = dist(*camera, *building)

        tg = self.data.iloc[index]['height'].astype('float32') / distance

        return image, distance, tg

train_data = Loader('train.csv')
train_loader = DataLoader(train_data)

criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

model.train()

idx = 0
for data, distance, label in train_loader:
    distance = torch.Tensor([[distance]])

    #output = model(data, distance)