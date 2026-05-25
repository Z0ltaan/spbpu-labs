import os
import gzip
import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import matplotlib.pyplot as plt
from torchsummary import summary


def load_mnist_images(filename):
    with gzip.open(filename, "rb") as f:
        data = np.frombuffer(f.read(), np.uint8, offset=16)
    return data.reshape(-1, 1, 28, 28).astype(np.float32) / 255.0


def load_mnist_labels(filename):
    with gzip.open(filename, "rb") as f:
        data = np.frombuffer(f.read(), np.uint8, offset=8)
    return data.astype(np.int64)


raw_dir = "./data/MNIST/raw"
train_img_path = os.path.join(raw_dir, "train-images-idx3-ubyte.gz")
train_lbl_path = os.path.join(raw_dir, "train-labels-idx1-ubyte.gz")
test_img_path = os.path.join(raw_dir, "t10k-images-idx3-ubyte.gz")
test_lbl_path = os.path.join(raw_dir, "t10k-labels-idx1-ubyte.gz")

print("=== ЗАПУСК КЛАССИФИКАТОРА MNIST ===")
print("Чтение локальных архивов напрямую...")

try:
    X_train = torch.tensor(load_mnist_images(train_img_path))
    y_train = torch.tensor(load_mnist_labels(train_lbl_path))
    X_test = torch.tensor(load_mnist_images(test_img_path))
    y_test = torch.tensor(load_mnist_labels(test_lbl_path))

    X_train = (X_train - 0.1307) / 0.3081
    X_test = (X_test - 0.1307) / 0.3081

    print(f"Обучение: {X_train.shape[0]} картинок, Тест: {X_test.shape[0]} картинок")
except Exception as e:
    print(f"Не удалось прочитать файлы: {e}")
    exit()

train_loader = torch.utils.data.DataLoader(
    torch.utils.data.TensorDataset(X_train, y_train), batch_size=64, shuffle=True
)
test_loader = torch.utils.data.DataLoader(
    torch.utils.data.TensorDataset(X_test, y_test), batch_size=1000, shuffle=False
)


class MNISTCNN(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(in_channels=1, out_channels=16, kernel_size=3, padding=1)
        self.relu1 = nn.ReLU()
        self.pool = nn.MaxPool2d(kernel_size=2, stride=2)

        self.conv2 = nn.Conv2d(
            in_channels=16, out_channels=32, kernel_size=3, padding=1
        )
        self.relu2 = nn.ReLU()

        self.fc1 = nn.Linear(32 * 7 * 7, 128)
        self.relu3 = nn.ReLU()
        self.fc2 = nn.Linear(128, 10)

    def forward(self, x):
        x = self.pool(self.relu1(self.conv1(x)))
        x = self.pool(self.relu2(self.conv2(x)))
        x = x.view(-1, 32 * 7 * 7)
        x = self.relu3(self.fc1(x))
        x = self.fc2(x)
        return x


device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model = MNISTCNN().to(device)
summary(model, input_size=(1, 28, 28))
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

model.train()
for batch_idx, (data, target) in enumerate(train_loader):
    data, target = data.to(device), target.to(device)
    optimizer.zero_grad()
    output = model(data)
    loss = criterion(output, target)
    loss.backward()
    optimizer.step()

    if batch_idx % 20 == 0:
        print(
            f"Батч {batch_idx:<4}/{len(train_loader)} | Текущий Loss: {loss.item():.4f}"
        )

model.eval()
correct = 0
with torch.no_grad():
    for data, target in test_loader:
        data, target = data.to(device), target.to(device)
        output = model(data)
        pred = output.argmax(dim=1, keepdim=True)
        correct += pred.eq(target.view_as(pred)).sum().item()

accuracy = 100.0 * correct / len(test_loader.dataset)
print(f"\nФинальное качество (Accuracy) на тесте: {accuracy:.2f}%")

sample_image = X_test[0]
sample_label = y_test[0].item()

fig1, axes1 = plt.subplots(4, 4, figsize=(6, 6))
filters = model.conv1.weight.data.cpu().numpy()
for i, ax in enumerate(axes1.flat):
    if i < len(filters):
        ax.imshow(filters[i, 0], cmap="viridis")
        ax.axis("off")
fig1.suptitle(
    "16 весовых фильтров слоя Conv1 (Размер ядер 3x3)", fontsize=11, fontweight="bold"
)
fig1.tight_layout()

img_tensor = sample_image.unsqueeze(0).to(device)
with torch.no_grad():
    feature_maps = model.relu1(model.conv1(img_tensor)).squeeze(0).cpu().numpy()

fig2, axes2 = plt.subplots(4, 4, figsize=(7, 7))
for i, ax in enumerate(axes2.flat):
    if i < len(feature_maps):
        ax.imshow(feature_maps[i], cmap="viridis")
        ax.axis("off")
fig2.suptitle(
    f"Карты признаков (Feature Maps) после Conv1 для цифры {sample_label}",
    fontsize=11,
    fontweight="bold",
)
fig2.tight_layout()

plt.show()
