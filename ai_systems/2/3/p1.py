#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import silhouette_score

df = pd.read_csv("pluton.csv")

max_iters = [1, 10, 50, 100, 200, 300]
results = []

for use_scaling in [False, True]:
    if use_scaling:
        X = StandardScaler().fit_transform(df)
        label = "standardized"
    else:
        X = df.values
        label = "not standardized"

    for m_iter in max_iters:
        kmeans = KMeans(
            n_clusters=3, max_iter=m_iter, init="random", n_init=10, random_state=42
        )
        kmeans.fit(X)
        sil = silhouette_score(X, kmeans.labels_)

        results.append(
            {"standardized": label, "iterations": m_iter, "silhouette score": sil}
        )

res_df = pd.DataFrame(results)

plt.figure(figsize=(7, 5.5))

for is_std in ["not standardized", "standardized"]:
    sub = res_df[res_df["standardized"] == is_std]
    plt.plot(sub["iterations"], sub["silhouette score"], marker="o", label=is_std)

plt.xlabel("iterations")
plt.ylabel("silhouette score")
plt.xticks(max_iters)
plt.grid(True)
plt.legend()
plt.tight_layout()

plt.show()
