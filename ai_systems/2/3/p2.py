#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans, DBSCAN, AgglomerativeClustering
from sklearn.metrics import silhouette_score
from sklearn.preprocessing import StandardScaler


def find_optimal_k(X, file_name):
    print(f"\n--- Анализ количества кластеров для {file_name} ---")
    sil_scores = []
    k_range = range(2, 11)

    for k in k_range:
        km = KMeans(n_clusters=k, random_state=42, n_init=10).fit(X)
        score = silhouette_score(X, km.labels_)
        sil_scores.append(score)
        print(f"Кластеров: {k} | Silhouette Score: {score:.4f}")

    optimal_k = k_range[np.argmax(sil_scores)]
    print(f"-> Оптимальное количество кластеров: {optimal_k}")
    return optimal_k


files = ["clustering_1.csv", "clustering_2.csv", "clustering_3.csv"]

for file in files:
    df = pd.read_csv(file, header=None, sep=r"\s+")
    X = StandardScaler().fit_transform(df)

    opt_k = find_optimal_k(X, file)

    fig, axs = plt.subplots(1, 3, figsize=(15, 4))
    fig.suptitle(f"Результаты для {file} (Оптимальное k={opt_k})", fontsize=12)

    km = KMeans(n_clusters=opt_k, random_state=42, n_init=10).fit(X)
    axs[0].scatter(X[:, 0], X[:, 1], c=km.labels_, cmap="viridis", s=15)
    axs[0].set_title(f"K-Means (k={opt_k})\nSil: {silhouette_score(X, km.labels_):.3f}")

    db = DBSCAN(eps=0.3, min_samples=5).fit(X)
    n_clusters_db = len(set(db.labels_)) - (1 if -1 in db.labels_ else 0)
    sil_db = silhouette_score(X, db.labels_) if n_clusters_db > 1 else -1
    axs[1].scatter(X[:, 0], X[:, 1], c=db.labels_, cmap="tab10", s=15)
    axs[1].set_title(f"DBSCAN (нашел кластеров: {n_clusters_db})\nSil: {sil_db:.3f}")

    linkage_type = "ward" if file == "clustering_1.csv" else "single"
    hc = AgglomerativeClustering(n_clusters=opt_k, linkage=linkage_type).fit(X)
    axs[2].scatter(X[:, 0], X[:, 1], c=hc.labels_, cmap="plasma", s=15)
    axs[2].set_title(
        f"Hierarchical (k={opt_k}, {linkage_type})\nSil: {silhouette_score(X, hc.labels_):.3f}"
    )

    plt.tight_layout()
    plt.show()

# import pandas as pd
# import matplotlib.pyplot as plt
# from sklearn.cluster import KMeans, DBSCAN, AgglomerativeClustering
# from sklearn.metrics import silhouette_score
# from sklearn.preprocessing import StandardScaler
#
# files = ["clustering_1.csv", "clustering_2.csv", "clustering_3.csv"]
#
# for file in files:
#     df = pd.read_csv(file, header=None, sep=r"\s+")
#     X = StandardScaler().fit_transform(df)
#
#     fig, axs = plt.subplots(1, 3, figsize=(15, 4))
#     fig.suptitle(f"file: {file}", fontsize=14)
#
#     km = KMeans(n_clusters=3, random_state=42, n_init=10).fit(X)
#     axs[0].scatter(X[:, 0], X[:, 1], c=km.labels_, cmap="viridis")
#     axs[0].set_title(
#         f"k-Means\nsilhouette score: {silhouette_score(X, km.labels_):.3f}"
#     )
#
#     db = DBSCAN(eps=0.3, min_samples=5).fit(X)
#     sil_db = silhouette_score(X, db.labels_) if len(set(db.labels_)) > 1 else -1
#     axs[1].scatter(X[:, 0], X[:, 1], c=db.labels_, cmap="tab10")
#     axs[1].set_title(f"DBSCAN\nsilhouette score: {sil_db:.3f}")
#
#     hc = AgglomerativeClustering(n_clusters=3, linkage="ward").fit(X)
#     axs[2].scatter(X[:, 0], X[:, 1], c=hc.labels_, cmap="plasma")
#     axs[2].set_title(
#         f"hierarchical\nsilhouette score: {silhouette_score(X, hc.labels_):.3f}"
#     )
#
#     plt.show()
