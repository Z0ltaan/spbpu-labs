#!/usr/bin/env python3

import pandas as pd
import statsmodels.api as sm

try:
    data = pd.read_csv("cygage.txt", sep=r"\s+")
except FileNotFoundError:
    print("Файл cygage.txt не найден.")
    exit()

X_single = data["Depth"]
X = sm.add_constant(X_single)
y = data["calAge"]
weights = data["Weight"]

model_ols = sm.OLS(y, X).fit()
model_wls = sm.WLS(y, X, weights=weights).fit()

print("\n" + "=" * 60)
print(f"Построены две линейные зависимости calAge от Depth:")
print(
    f"1. Равновесная модель (OLS): R² = {model_ols.rsquared:.4f}, Intercept = {model_ols.params.iloc[0]:.2f}, Slope = {model_ols.params.iloc[1]:.2f}"
)
print(
    f"2. Взвешенная модель (WLS):  R² = {model_wls.rsquared:.4f}, Intercept = {model_wls.params.iloc[0]:.2f}, Slope = {model_wls.params.iloc[1]:.2f}"
)
print("=" * 60)
