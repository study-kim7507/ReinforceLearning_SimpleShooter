import collections
import random
import torch
import numpy as np

class ReplayBuffer():
    def __init__(self, buffer_limit=1000):
        self.buffer = collections.deque(maxlen=buffer_limit)

    def put(self, transition):
        self.buffer.append(transition)

    def sample(self, n):
        mini_batch = random.sample(self.buffer, n)
        s_lst, a_lst, r_lst, s_prime_lst, done_mask_lst = [], [], [], [], []

        for transition in mini_batch:
            s, a, r, s_prime, done_mask = transition
            s_lst.append(s)
            a_lst.append([a])
            r_lst.append([r])
            s_prime_lst.append(s_prime)
            done_mask_lst.append([done_mask])

        # Convert lists to numpy arrays
        s_array = np.array(s_lst)
        a_array = np.array(a_lst)
        r_array = np.array(r_lst)
        s_prime_array = np.array(s_prime_lst)
        done_mask_array = np.array(done_mask_lst)

        # Convert numpy arrays to PyTorch tensors
        return torch.tensor(s_array, dtype=torch.float), torch.tensor(a_array, dtype=torch.int64), \
            torch.tensor(r_array), torch.tensor(s_prime_array, dtype=torch.float), \
            torch.tensor(done_mask_array)

    def size(self):
        return len(self.buffer)
