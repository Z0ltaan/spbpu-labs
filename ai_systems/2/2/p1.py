#!/usr/bin/env python3

import os
import torch
import torch.nn as nn
import torch.optim as optim
import pandas as pd
import numpy as np

def load_or_create_datasets():
    files = ["nn_0.csv", "nn_1.csv"]
    data_dict = {}
    for file in files:
        if os.path.exists(file):
            df = pd.read_csv(file, sep=None, engine="python")
            df.columns = df.columns.str.strip()
            if "x1" in df.columns and "x2" in df.columns:
                X = torch.tensor(df[["x1", "x2"]].values, dtype=torch.float32)
            else:
                X = torch.tensor(df.iloc[:, :2].values, dtype=torch.float32)

            if "class" in df.columns:
                y = torch.tensor(df["class"].values, dtype=torch.float32).unsqueeze(1)
            else:
                y = torch.tensor(df.iloc[:, -1].values, dtype=torch.float32).unsqueeze(1)

            data_dict[file] = (X, y)

            print(f" Успешно загружен файл: {file} ({len(df)} строк)")
        else:
            print(f"Файл {file} не найден. Используются демонстрационные данные.")

    return data_dict


class SingleNeuron(nn.Module):
    def __init__(self, activation_name="tanh"):
        super().__init__()
        self.linear = nn.Linear(2, 1)
        self.activation_name = activation_name.lower()

    def forward(self, x):
        out = self.linear(x)
        if self.activation_name == "tanh":
            return torch.tanh(out)
        elif self.activation_name == "sigmoid":
            return 2.0 * torch.sigmoid(out) - 1.0
        elif self.activation_name == "none":
            return out
        else:
            raise ValueError(f"Неизвестная активация: {self.activation_name}")


def run_experiment(X, y, filename, act, opt_name, max_epochs=15000, lr=0.05):
    model = SingleNeuron(activation_name=act)
    criterion = nn.MSELoss()

    if opt_name.lower() == "adam":
        optimizer = optim.Adam(model.parameters(), lr=lr)
    elif opt_name.lower() == "sgd":
        optimizer = optim.SGD(model.parameters(), lr=lr)
    elif opt_name.lower() == "sgd_momentum":
        optimizer = optim.SGD(model.parameters(), lr=lr, momentum=0.9)
    elif opt_name.lower() == "rmsprop":
        optimizer = optim.RMSprop(model.parameters(), lr=lr)
    elif opt_name.lower() == "adagrad":
        optimizer = optim.Adagrad(model.parameters(), lr=lr)
    else:
        raise ValueError(f"Неизвестный оптимизатор: {opt_name}")

    converged_epoch = max_epochs
    has_converged = False

    for epoch in range(1, max_epochs + 1):
        model.train()
        optimizer.zero_grad()

        predictions = model(X)
        loss = criterion(predictions, y)
        loss.backward()
        optimizer.step()

        preds_bin = torch.where(predictions >= 0, 1.0, -1.0)
        accuracy = (preds_bin == y).float().mean().item()

        if accuracy >= 1.0 and not has_converged:
            converged_epoch = epoch
            has_converged = True

    final_loss = loss.item()
    final_acc = accuracy * 100

    print(
        f"[{filename}] Активация: {act:<7} | Оптимизатор: {opt_name:<12} -> "
        f"Итог Accuracy: {final_acc:>5.1f}% | Loss: {final_loss:.4f} | "
        f"Сходимость на эпохе: {converged_epoch if has_converged else 'Не сошелся'}"
    )

    return {
        "dataset": filename,
        "activation": act,
        "optimizer": opt_name,
        "accuracy": final_acc,
        "loss": final_loss,
        "epoch": converged_epoch if has_converged else max_epochs,
    }


if __name__ == "__main__":
    torch.manual_seed(42)
    np.random.seed(42)

    datasets = load_or_create_datasets()

    activations = ["tanh", "sigmoid", "none"]
    optimizers = ["adam", "sgd", "sgd_momentum", "rmsprop", "adagrad"]

    results = []

    print("\n--- СТАРТ ОБУЧЕНИЯ ---")
    for file_name, (X, y) in datasets.items():
        for act in activations:
            for opt in optimizers:
                res = run_experiment(X, y, file_name, act, opt, lr=0.05)
                results.append(res)


