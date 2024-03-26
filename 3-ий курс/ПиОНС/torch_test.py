import torch
import torch.nn as nn
import sys
import pandas as pd
from tqdm import tqdm
import numpy as np
from torch.utils.data import Dataset, DataLoader
import torchvision.transforms as transforms
import matplotlib.pyplot as plt

model = nn.Sequential(
    nn.Linear(28 * 28, 512),
    nn.ReLU(),
    nn.Dropout(0.2),

    nn.Linear(512, 512),
    nn.ReLU(),
    nn.Dropout(0.2),

    nn.Linear(512, 10),
)

model.to('cuda:0')

criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

class TrainMNIST(Dataset):
    
    def __init__(self, file_path, transform=None):
        self.data = pd.read_csv(file_path)
        self.transform = transform
        
    def __len__(self):
        return len(self.data)
    
    def __getitem__(self, index):
        # load image as ndarray type (Height * Width * Channels)
        # be carefull for converting dtype to np.uint8 [Unsigned integer (0 to 255)]
        # in this example, we use ToTensor(), so we define the numpy array like (H, W, C)
        image = self.data.iloc[index, 1:].values.astype(np.uint8).reshape((28, 28, 1))
        label = self.data.iloc[index, 0]
        
        if self.transform is not None:
            image = self.transform(image)
            
        return image, label
class TestMNIST(Dataset):
    
    def __init__(self, file_path, transform=None):
        self.data = pd.read_csv(file_path)
        self.transform = transform
        
    def __len__(self):
        return len(self.data)
    
    def __getitem__(self, index):
        # load image as ndarray type (Height * Width * Channels)
        # be carefull for converting dtype to np.uint8 [Unsigned integer (0 to 255)]
        # in this example, we use ToTensor(), so we define the numpy array like (H, W, C)
        image = self.data.iloc[index, 1:].values.astype(np.uint8).reshape((28, 28, 1))
        label = self.data.iloc[index, 0]
        
        if self.transform is not None:
            image = self.transform(image)
            
        return image, label

batch_size = 100

train_data = TrainMNIST(sys.path[0] + "/mnist_train.csv", transform=transforms.ToTensor())
test_data = TestMNIST(sys.path[0] + "/mnist_test.csv", transform=transforms.ToTensor())

train_loader = torch.utils.data.DataLoader(train_data, batch_size=batch_size)
valid_loader = torch.utils.data.DataLoader(train_data, batch_size=batch_size)
test_loader = torch.utils.data.DataLoader(test_data, batch_size=len(test_data))

num_epoch = 10

for epoch in range(num_epoch):
    model.train()

    train_loss = 0.0

    for data, target in tqdm(train_loader):
        data = data.view(-1, 28 * 28)

        # clear the gradients of all optimized variables
        optimizer.zero_grad()
        # forward pass: compute predicted outputs by passing inputs to the model
        output = model(data.to(device='cuda:0'))
        # calculate the loss
        loss = criterion(output, target.to(device='cuda:0'))
        # backward pass: compute gradient of the loss with respect to model parameters
        loss.backward()
        # perform a single optimization step (parameter update)
        optimizer.step()

        train_loss += loss.item() * data.size(0)
    train_loss = train_loss / len(train_loader.dataset)
    print(train_loss)

data, target = next(iter(test_loader))
target = target.to(device='cuda:0')
output = model(data.view(-1, 28 * 28).to(device='cuda:0'))
_, predicted = torch.max(output, 1)
predicted = predicted.to(device='cuda:0')
accuracy = (predicted == target).sum().item() / target.size(0)

print(f'Accuracy: {accuracy}')

# for data, target in test_loader:
#     output = model(data.view(-1, 28 * 28).to(device='cuda:0'))
#     output = torch.softmax(output, 1)

#     print(output, torch.max(output, 1))

#     pred = torch.max(output, 1)[1][0].item()
#     target = target[0].item()

#     data = data.view(28, 28).tolist()

#     print(f'{pred} : {target}')

#     plt.imshow(data, cmap='gray')
#     plt.show()
    