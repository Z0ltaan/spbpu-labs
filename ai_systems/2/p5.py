#!/usr/bin/env python3

import pandas as pd
import numpy as np
import statsmodels.api as sm
import matplotlib.pyplot as plt

try:
    data = pd.read_csv("eustock.csv")
except FileNotFoundError:
    print("Файл eustock.csv не найден.")
    exit()

data["Day"] = np.arange(len(data))
X = sm.add_constant(data["Day"])

plt.figure(figsize=(12, 6))
plt.grid(True, linestyle="--", alpha=0.5)

colors = {"DAX": "#1f77b4", "SMI": "#ff7f0e", "CAC": "#2ca02c", "FTSE": "#d62728"}

for col in ["DAX", "SMI", "CAC", "FTSE"]:
    plt.plot(data["Day"], data[col], label=col, color=colors[col], alpha=0.8)

plt.title(
    "Ежедневные котировки фондовых бирж во времени", fontsize=13, fontweight="bold"
)
plt.xlabel("День", fontsize=11)
plt.ylabel("Значение индекса на момент закрытия", fontsize=11)
plt.legend(frameon=True, loc="upper left")
plt.tight_layout()
plt.savefig("eustock_trends.png", dpi=300)
plt.show()

print("РЕЗУЛЬТАТЫ ИНДИВИДУАЛЬНЫХ РЕГРЕССИЙ:")
print("-" * 65)
print(f"{'Биржа':<15} | {'Динамика':<20} | {'R²':<12}")
print("-" * 65)

slopes = {}
for col in ["DAX", "SMI", "CAC", "FTSE"]:
    y = data[col]
    model = sm.OLS(y, X).fit()
    slope = model.params["Day"]
    slopes[col] = slope
    print(f"{col:<15} | {slope:<20.4f} | {model.rsquared:.4f}")

data_long = pd.melt(
    data,
    id_vars=["Day"],
    value_vars=["DAX", "SMI", "CAC", "FTSE"],
    var_name="Index",
    value_name="Value",
)

X_all = sm.add_constant(data_long["Day"])
y_all = data_long["Value"]
model_all = sm.OLS(y_all, X_all).fit()

print("-" * 65)
print(
    f"Общая модель (All) | {model_all.params['Day']:<20.4f} | {model_all.rsquared:.4f}"
)
print("-" * 65)

fastest_index = max(slopes, key=slopes.get)
print(f"\nНАИБОЛЬШАЯ ДИНАМИКА ЧИСЛЕННО: {fastest_index}")
