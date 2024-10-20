import torch.nn as nn
import torch.nn.functional as F
import random

class Qnet(nn.Module):
    def __init__(self):
        super(Qnet, self).__init__()

        self.fc1 = nn.Linear(7, 128)
        self.fc2 = nn.Linear(128, 512)
        self.fc3 = nn.Linear(512, 1024)
        self.fc4 = nn.Linear(1024, 512)
        self.fc5 = nn.Linear(512, 128)
        self.fc6 = nn.Linear(128, 33)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = F.relu(self.fc3(x))
        x = F.relu(self.fc4(x))
        x = F.relu(self.fc5(x))
        x = self.fc6(x)
        return x

    def sample_action(self, obs, epsilon):
        if random.random() < epsilon:
            return random.randint(0, 32)
        else:
            out = self.forward(obs)
            return out.argmax().item()


