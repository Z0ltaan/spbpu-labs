#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.svm import SVR
from sklearn.metrics import mean_squared_error

try:
    data = pd.read_csv("svmdata6.txt", sep=r"\s+")
except FileNotFoundError:
    print("Файл svmdata6.txt не найден.")
    exit()

X = data[["X"]]
y = data["Y"]
epsilon_values = np.arange(0.0, 1.01, 0.02)
mse_errors = []

for eps in epsilon_values:
    svr = SVR(kernel="rbf", C=1.0, epsilon=eps)
    svr.fit(X, y)

    y_pred = svr.predict(X)
    mse = mean_squared_error(y, y_pred)
    mse_errors.append(mse)

plt.figure(figsize=(9, 5))
plt.grid(True, linestyle="--", alpha=0.5)

plt.plot(
    epsilon_values,
    mse_errors,
    color="darkviolet",
    linewidth=2.5,
    marker="o",
    markersize=4,
)

plt.title(
    "Зависимость MSE от параметра $\epsilon$ в модели SVR",
    fontsize=12,
    fontweight="bold",
)
plt.xlabel("Параметр $\epsilon$ (ширина трубки)", fontsize=11)
plt.ylabel("Среднеквадратичная ошибка (MSE) на обучении", fontsize=11)

plt.tight_layout()
plt.savefig("svr_epsilon_dependency.png", dpi=300)
plt.show()
