#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.ensemble import AdaBoostClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import cross_val_score
from sklearn.naive_bayes import GaussianNB
from sklearn.preprocessing import StandardScaler
from sklearn.tree import DecisionTreeClassifier

data = pd.read_csv("vehicle.csv")
X = data.drop(columns=["Class"])
y = data["Class"]

scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

base_models = {
    "Decision Tree": DecisionTreeClassifier(max_depth=5, random_state=42),
    "Logistic Regression": LogisticRegression(max_iter=2000, random_state=42),
    "Naive Bayes": GaussianNB(),
}

n_estimators_range = [1, 11, 21, 31, 41, 51, 61, 75, 81, 91, 100]
results = {model_name: [] for model_name in base_models}

for model_name, base_clf in base_models.items():
    for n_est in n_estimators_range:
        try:
            boost_clf = AdaBoostClassifier(
                estimator=base_clf,
                n_estimators=n_est,
                random_state=42,
            )
            scores = cross_val_score(boost_clf, X_scaled, y, cv=5, scoring="accuracy")
            results[model_name].append(np.mean(scores))
        except ValueError:
            results[model_name].append(np.nan)

plt.figure(figsize=(10, 6))
markers = {
    "Decision Tree": "s",
    "Logistic Regression": "^",
    "Naive Bayes": "X",
}
colors = {
    "Decision Tree": "darkorange",
    "Logistic Regression": "crimson",
    "Naive Bayes": "purple",
}

for model_name in base_models:
    if not np.isnan(results[model_name]).all():
        plt.plot(
            n_estimators_range,
            results[model_name],
            marker=markers[model_name],
            color=colors[model_name],
            linewidth=2,
            label=model_name,
        )

plt.title("Зависимость Accuracy от количества базовых алгоритмов в бустинге")
plt.xlabel("Количество базовых классификаторов ($N$)")
plt.ylabel("Качество классификации (Accuracy)")
plt.xticks(n_estimators_range)
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()
