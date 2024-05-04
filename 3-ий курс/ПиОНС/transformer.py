import torch
import torch.nn as nn
import math

class InputEmbedding(nn.Module):
    def __init__(self, emb_depth, voc_size):
        super().__init__()
        self.emb_depth = emb_depth
        self.voc_size = voc_size
        self.embedding = nn.Embedding(self.voc_size, self.emb_depth)

    def forward(self, x : torch.Tensor):
        self.embedding(x) * math.sqrt(self.emb_depth)

class PositionEncoding(nn.Module):
    def __init__(self, emb_depth, voc_size, dropout=0.1):
        super().__init__(self)

        self.emb_depth = emb_depth
        self.voc_size = voc_size
        self.dropout = nn.Dropout(dropout)

        pe = torch.zeros(voc_size, emb_depth)

        pos = torch.arange(0, voc_size, dtype=torch.float).unsqueeze(1)
        div_term = torch.exp( torch.arange(0, emb_depth, 2) * -math.log(10000) / emb_depth )
        
        pe[:, 0::2] = torch.sin(pos * div_term)
        pe[:, 1::2] = torch.cos(pos * div_term)

        self.pe = pe.unsqueeze(0)

        self.register_buffer('pe', pe)

    def forward(self, x : torch.Tensor):
        x = x + (self.pe[:, x.shape[1], :]).requires_grad_(False)
        return self.dropout(x)
    
class NormalizationLayer(nn.Module):
    def __init__(self, eps : float = 10 ** -6):
        super().__init__()

        self.eps = eps
        self.alpha = nn.Parameter(torch.ones(1))
        self.bias = nn.Parameter(torch.ones(1))

    def forward(self, x : torch.Tensor):
        mean = x.mean(1, keepdim=True)
        std = x.std(1, keepdim=True)

        return (x - mean) / (std + self.eps) + self.bias
    
class FullyConnectedLayer(nn.Module):
    def __init__(self, emb_depth, num_hidden, dropout):
        super().__init__()

        self.fc1 = nn.Linear(emb_depth, num_hidden)
        self.dropout = nn.Dropout(dropout)

        self.fc2 = nn.Linear(num_hidden, emb_depth)

    def forward(self, x : torch.Tensor):
        return self.fc2(self.dropout(torch.relu(self.fc1(x))))
    
