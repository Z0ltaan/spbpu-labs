#!/usr/bin/env python3

import os
import torch
import torch.nn as nn
import torch.optim as optim
import pandas as pd
import numpy as np


def load_nn_1():
    file = "nn_1.csv"
    if os.path.exists(file):
        df = pd.read_csv(file, sep=None, engine="python")
        df.columns = df.columns.str.strip()
    else:
        np.random.seed(42)
        r = np.random.uniform(0, 1.5, 300)
        theta = np.random.uniform(0, 2 * np.pi, 300)
        x1 = r * np.cos(theta)
        x2 = r * np.sin(theta)
        cl = np.where(r < 0.4, 1.0, -1.0)
        df = pd.DataFrame({"x1": x1, "x2": x2, "class": cl})

    if "x1" in df.columns and "x2" in df.columns:
        X = torch.tensor(df[["x1", "x2"]].values, dtype=torch.float32)
    else:
        X = torch.tensor(df.iloc[:, :2].values, dtype=torch.float32)

    if "class" in df.columns:
        y = torch.tensor(df["class"].values, dtype=torch.float32).unsqueeze(1)
    else:
        y = torch.tensor(df.iloc[:, -1].values, dtype=torch.float32).unsqueeze(1)

    return X, y


class FlexibleMLP(nn.Module):
    def __init__(self, activation_name="relu"):
        super().__init__()
        self.hidden = nn.Linear(2, 16)
        self.output = nn.Linear(16, 1)
        self.output_act = nn.Tanh()

        act_str = activation_name.lower()
        if act_str == "relu":
            self.hidden_act = nn.ReLU()
        elif act_str == "tanh":
            self.hidden_act = nn.Tanh()
        elif act_str == "sigmoid":
            self.hidden_act = nn.Sigmoid()
        else:
            raise ValueError(f"Неизвестная активация: {activation_name}")

    def forward(self, x):
        x = self.hidden(x)
        x = self.hidden_act(x)
        x = self.output(x)
        x = self.output_act(x)
        return x


def run_mlp_experiment(X, y, act, opt_name, max_epochs=300, lr=0.02):
    model = FlexibleMLP(activation_name=act)
    # print(model)
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

        if accuracy >= 0.999 and not has_converged:
            converged_epoch = epoch
            has_converged = True
            break

    final_loss = loss.item()
    final_acc = accuracy * 100

    print(
        f"Скрытый слой: {act:<7} | lr = {lr:<5} | Оптимизатор: {opt_name:<12} -> "
        f"Итог Accuracy: {final_acc:>5.1f}% | Loss: {final_loss:.4f} | "
        f"Сходимость (>=99.9%): {converged_epoch if has_converged else 'Не сошелся'}"
    )

    return {
        "activation": act,
        "optimizer": opt_name,
        "accuracy": final_acc,
        "loss": final_loss,
        "epoch": converged_epoch if has_converged else max_epochs,
    }


if __name__ == "__main__":
    torch.manual_seed(42)
    np.random.seed(42)

    X, y = load_nn_1()

    activations = ["relu", "tanh", "sigmoid"]
    optimizers = ["adam", "sgd", "sgd_momentum", "rmsprop", "adagrad"]

    print("\n--- СТАРТ ТЕСТИРОВАНИЯ ---")
    results = []
    lrs = [0.0001, 0.001, 0.005, 0.01, 0.05, 0.1, 0.5, 1.0]
    for act in activations:
        for opt in optimizers:
            for lr in lrs:
                res = run_mlp_experiment(X, y, act, opt, max_epochs=500, lr=lr)
                results.append(res)
