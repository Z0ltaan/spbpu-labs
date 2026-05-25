#!/usr/bin/env python3

import pandas as pd
import numpy as np
import statsmodels.api as sm
import matplotlib.pyplot as plt

try:
    data = pd.read_csv("cars.csv")
except FileNotFoundError:
    try:
        data = pd.read_csv("cars.txt")
    except FileNotFoundError:
        print("Файл с данными не найден.")
        exit()

X_single = data["speed"]
X = sm.add_constant(X_single)
y = data["dist"]

model = sm.OLS(y, X).fit()
speed_target = 40
dist_pred_40 = model.predict([1, speed_target])[0]

plt.figure(figsize=(9, 6))
plt.grid(True, linestyle="--", alpha=0.5)

plt.scatter(
    X_single,
    y,
    color="darkblue",
    edgecolors="black",
    alpha=0.7,
    label="Фактические данные (1920 г.)",
)

x_plot = np.linspace(X_single.min(), speed_target + 2, 100)
X_plot = sm.add_constant(x_plot)
y_plot = model.predict(X_plot)
plt.plot(
    x_plot,
    y_plot,
    color="crimson",
    linewidth=2,
    label=f"Линия регрессии (R² = {model.rsquared:.4f})",
)

plt.scatter(
    speed_target,
    dist_pred_40,
    color="gold",
    edgecolor="black",
    s=120,
    zorder=5,
    label=f"Прогноз для 40 миль/ч ({dist_pred_40:.2f} футов)",
)

plt.title(
    "Зависимость тормозного пути от скорости автомобиля", fontsize=12, fontweight="bold"
)
plt.xlabel("Скорость (speed), мили в час", fontsize=11)
plt.ylabel("Тормозной путь (dist), футы", fontsize=11)
plt.xlim(left=0)
plt.ylim(bottom=0)
plt.legend(frameon=True, loc="upper left")

plt.tight_layout()
plt.savefig("cars_regression.png", dpi=300)
plt.show()

print(f"ОЦЕНКА ТОРМОЗНОГО ПУТИ ПРИ 40 милях/ч: {dist_pred_40:.2f} футов")
