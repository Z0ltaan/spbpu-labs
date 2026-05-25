#!/usr/bin/env python3

import pandas as pd
import statsmodels.api as sm

try:
    data = pd.read_csv("reglab1.txt", sep=r"\s+")
except FileNotFoundError:
    print("Файл reglab1.txt не найден.")
    exit()

models_to_check = [
    ("z", ["x", "y"]),
    ("x", ["z", "y"]),
    ("y", ["z", "x"]),
    ("x", ["z"]),
    ("x", ["y"]),
    ("y", ["x"]),
    ("y", ["z"]),
    ("z", ["x"]),
    ("z", ["y"]),
]

results = []

print("РЕЗУЛЬТАТЫ СРАВНЕНИЯ МОДЕЛЕЙ:\n" + "-" * 40)

for target, features in models_to_check:
    X = data[features]
    y = data[target]

    X_with_const = sm.add_constant(X)
    model = sm.OLS(y, X_with_const).fit()
    model_name = f"{target}({', '.join(features)})"

    results.append(
        {"name": model_name, "R2": model.rsquared, "summary": model.summary()}
    )

    print(f"Модель {model_name:<10} | R^2 = {model.rsquared:.4f}")

best_model = max(results, key=lambda item: item["R2"])

print("-" * 40)
print(
    f"НАИБОЛЕЕ ПОДХОДЯЩАЯ МОДЕЛЬ: {best_model['name']} с R^2 = {best_model['R2']:.4f}"
)
print("-" * 40)
