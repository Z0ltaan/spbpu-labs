#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import dendrogram, linkage
from sklearn.preprocessing import StandardScaler
from sklearn.impute import SimpleImputer

df = pd.read_csv("votes.csv")
state_labels = [f"Штат {i}" for i in df.index]
imputer = SimpleImputer(strategy="mean")
df_imputed = imputer.fit_transform(df)
X_scaled = StandardScaler().fit_transform(df_imputed)
Z = linkage(X_scaled, method="ward")

plt.figure(figsize=(14, 7))
dendrogram(
    Z,
    labels=state_labels,
    leaf_rotation=90,
    leaf_font_size=10,
    color_threshold=6.0,
)

plt.title(
    "Дендрограмма числa голосов, поданных за республиканцев на выборах с 1856 по 1976 год",
    fontsize=14,
)
plt.xlabel("Штаты")
plt.ylabel("критерий Уорда")
plt.grid(axis="y", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.show()

# import pandas as pd
# import matplotlib.pyplot as plt
# from scipy.cluster.hierarchy import dendrogram, linkage
# from sklearn.preprocessing import StandardScaler
#
# df = pd.read_csv("votes.csv", index_col=0)
# X_scaled = StandardScaler().fit_transform(df)
# Z = linkage(X_scaled, method="ward")
#
# plt.figure(figsize=(14, 7))
# dendrogram(
#     Z,
#     labels=df.index,
#     leaf_rotation=90,
#     leaf_font_size=10,
#     color_threshold=6.0,
# )
#
# plt.title(
#     "Дендрограмма числa голосов, поданных за республиканцев на выборах с 1856 по 1976 год",
#     fontsize=14,
# )
# plt.xlabel("Штаты")
# plt.ylabel("критерий Уорда")
# plt.grid(axis="y", linestyle="--", alpha=0.7)
# plt.tight_layout()
# plt.show()
