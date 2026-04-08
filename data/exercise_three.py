import torch
import pandas as pd
import matplotlib.pyplot as plt

dataset = pd.read_csv("dataset.csv")

# print(dataset.shape)
# print(dataset.head(5))
# print(dataset.info())

# dataset.plot.area(subplots=True)
# plt.show()

x = torch.rand(5, 3)
print(x)