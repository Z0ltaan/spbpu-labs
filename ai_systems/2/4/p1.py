#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.ensemble import BaggingClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import cross_val_score
from sklearn.naive_bayes import GaussianNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier

data = pd.read_csv("glass.csv")
X = data.drop(columns=["Id", "Type"], errors="ignore")
y = data["Type"]

scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

base_models = {
    "Decision Tree": DecisionTreeClassifier(random_state=42),
    "KNN": KNeighborsClassifier(n_neighbors=3),
    "Logistic Regression": LogisticRegression(max_iter=1000, random_state=42),
    "SVM (RBF)": SVC(kernel="rbf", probability=True, random_state=42),
    "Naive Bayes": GaussianNB(),
}


n_estimators_range = [
    1,
    5,
    10,
    15,
    20,
    25,
    30,
    35,
    40,
    45,
    50,
    55,
    60,
    65,
    70,
    75,
    80,
    85,
    90,
    95,
    100,
]
results = {model_name: [] for model_name in base_models}

print("Запуск вычислений...")
for model_name, base_clf in base_models.items():
    print(f"Оценка ансамбля на базе: {model_name}")

    for n_est in n_estimators_range:
        bagging_clf = BaggingClassifier(
            estimator=base_clf, n_estimators=n_est, random_state=42, n_jobs=-1
        )

        scores = cross_val_score(bagging_clf, X_scaled, y, cv=5, scoring="accuracy")
        results[model_name].append(np.mean(scores))

plt.figure(figsize=(12, 7))

markers = {
    "Decision Tree": "o",
    "KNN": "s",
    "Logistic Regression": "^",
    "SVM (RBF)": "D",
    "Naive Bayes": "X",
}

colors = {
    "Decision Tree": "forestgreen",
    "KNN": "royalblue",
    "Logistic Regression": "crimson",
    "SVM (RBF)": "darkorange",
    "Naive Bayes": "purple",
}

for model_name in base_models:
    plt.plot(
        n_estimators_range,
        results[model_name],
        marker=markers[model_name],
        color=colors[model_name],
        linewidth=2,
        label=model_name,
    )

plt.title(
    "Зависимость Accuracy от количества базовых алгоритмов в бэггинге",
    fontsize=14,
    pad=15,
)
plt.xlabel("Количество базовых классификаторов ($N$)", fontsize=12)
plt.ylabel("Качество классификации (Accuracy)", fontsize=12)
plt.xticks(n_estimators_range)
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend(fontsize=11, loc="best")

plt.tight_layout()
plt.show()
