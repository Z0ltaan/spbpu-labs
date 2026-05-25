#!/usr/bin/env python3

import pandas as pd
import numpy as np
import itertools
import statsmodels.api as sm

try:
    data = pd.read_csv("reglab.txt", sep=r"\s+")
except FileNotFoundError:
    print("Файл reglab.txt не найден.")
    exit()

features = ["x1", "x2", "x3", "x4"]
target = "y"
best_models = {}

for k in range(0, 4):
    if k == 0:
        X = np.ones((len(data), 1))
        y = data[target]
        model = sm.OLS(y, X).fit()
        best_models[0] = {
            "features": [],
            "RSS": model.ssr,
            "Adj_R2": model.rsquared_adj,
            "AIC": model.aic,
        }
    else:
        best_rss = float("inf")
        best_subset = None
        best_model = None

        for combo in itertools.combinations(features, k):
            X = sm.add_constant(data[list(combo)])
            y = data[target]
            model = sm.OLS(y, X).fit()

            if model.ssr < best_rss:
                best_rss = model.ssr
                best_subset = list(combo)
                best_model = model

        best_models[k] = {
            "features": best_subset,
            "RSS": best_rss,
            "Adj_R2": best_model.rsquared_adj,
            "AIC": best_model.aic,
        }

print(
    f"{'k':<3} | {'Подмножество признаков':<25} | {'RSS':<8} | {'Adj R²':<8}"
)
print("-" * 62)
for k, metrics in best_models.items():
    feats_str = str(metrics["features"])
    print(
        f"{k:<3} | {feats_str:<25} | {metrics['RSS']:.4f} | {metrics['Adj_R2']:.4f}"
    )
