import torch
import tomli_w
import torch.nn as nn
import torch.optim as optim
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import LabelEncoder, StandardScaler

data = pd.read_csv("../data/obd_data.csv")
data.plot.area(subplots=True)

epochCount = 60
coefTrain = 0.8
sizeColumn = int((data.size / data.columns.size))
sizeTrain = int(sizeColumn * coefTrain)
sizeTest = sizeColumn - sizeTrain

le = LabelEncoder()
le.fit(["GENTLE", "NORMAL", "VIOLENT"])
labels = np.array(le.transform(data["LABEL"]))
labelsTrain = np.delete(labels, np.arange(sizeTrain, sizeColumn))
labelsTest = np.delete(labels, np.arange(sizeTrain))

scaler = StandardScaler()
signs = data.drop(columns="LABEL")
scaler.fit(signs)

mean = np.array(scaler.mean_)
std = np.array(scaler.scale_)

signs = np.array(scaler.fit_transform(signs))
signsTrain = np.delete(signs, np.arange(sizeTrain, sizeColumn), axis=0)
signsTest = np.delete(signs, np.arange(sizeTrain), axis=0)

class Net(nn.Module):
    def __init__(self):
        super().__init__()

        self.fc1 = nn.Linear(6, 64)
        self.fc2 = nn.Linear(64, 32)
        self.fc3 = nn.Linear(32, 3)
        self.relu = nn.ReLU()
        self.dropout = nn.Dropout()

    def forward(self, input):
        f1 = self.dropout(self.relu(self.fc1(input)))
        f2 = self.dropout(self.relu(self.fc2(f1)))
        output = self.fc3(f2)

        return output
    
net = Net()
optimizer = optim.Adam(net.parameters(), lr=0.001)
criterion = nn.CrossEntropyLoss()

for epoch in range(epochCount):
    net.train()

    for i in range(sizeTrain):
        optimizer.zero_grad()

        output = net(torch.tensor(signsTrain[i], dtype=torch.float32))
        target = torch.tensor(labelsTrain[i], dtype=torch.long)

        loss = criterion(output, target)
        loss.backward()
        optimizer.step()

losses = []
accuracies = []
predictions = 0
net.eval()

for i in range(sizeTest):
    output = net(torch.tensor(signsTest[i], dtype=torch.float32))
    target = torch.tensor(labelsTest[i], dtype=torch.long)

    loss = criterion(output, target)
    losses.append(loss.item())

    if (torch.argmax(output) == target):
        predictions += 1

    accuracies.append((predictions / (i + 1)) * 100)

fig, axes = plt.subplots()

axes.plot(np.arange(sizeTest), losses, label="Loss")
axes.plot(np.arange(sizeTest), accuracies, label="Accuracy")
axes.set_xlabel("Индекс тестового элемента")
axes.grid()
axes.legend()

plt.show()

torch.onnx.export(
    net, 
    torch.rand(6, dtype=torch.float32),
    "driver_classifier.onnx",
    input_names=['features'],
    output_names=['class_scores'],
    opset_version=12
)

tomlData = {
    "mean": mean.tolist(), 
    "std": std.tolist(),
    "classes": ["GENTLE", "NORMAL", "VIOLENT"]
}

with open("normalization_params.toml", "wb") as tomlFile:
    tomli_w.dump(tomlData, tomlFile)