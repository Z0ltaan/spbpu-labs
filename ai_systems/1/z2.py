#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import (
    accuracy_score,
    confusion_matrix,
    roc_curve,
    auc,
    precision_recall_curve,
)
from sklearn.model_selection import train_test_split

np.random.seed(42)

# Класс -1
n1 = 20
mean1 = [22, 5]
std1 = 4
X_minus1 = np.random.normal(loc=mean1, scale=std1, size=(n1, 2))
y_minus1 = np.full(n1, -1)

# Класс 1
n2 = 80
mean2 = [10, 16]
std2 = 5
X_plus1 = np.random.normal(loc=mean2, scale=std2, size=(n2, 2))
y_plus1 = np.full(n2, 1)

X = np.vstack((X_minus1, X_plus1))
y = np.concatenate((y_minus1, y_plus1))

plt.figure(figsize=(8, 6))
plt.scatter(
    X_minus1[:, 0], X_minus1[:, 1], color="red", label="Класс -1 (n=20)", edgecolors="k"
)
plt.scatter(
    X_plus1[:, 0], X_plus1[:, 1], color="blue", label="Класс 1 (n=80)", edgecolors="k"
)
plt.title("Распределение точек по варианту")
plt.xlabel("X1")
plt.ylabel("X2")
plt.legend()
plt.grid(True, linestyle="--", alpha=0.6)
plt.show()

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.3, random_state=42
)

model = GaussianNB()
model.fit(X_train, y_train)

y_pred = model.predict(X_test)
y_probs = model.predict_proba(X_test)[:, 1]

print("--- Результаты классификации ---")
print(f"Accuracy (Точность): {accuracy_score(y_test, y_pred):.2f}")
print("Матрица ошибок (Confusion Matrix):")
print(confusion_matrix(y_test, y_pred))

fpr, tpr, _ = roc_curve(y_test, y_probs)
roc_auc = auc(fpr, tpr)
precision, recall, _ = precision_recall_curve(y_test, y_probs)

plt.figure(figsize=(12, 5))

# ROC-кривая
plt.subplot(1, 2, 1)
plt.plot(fpr, tpr, color="darkorange", lw=2, label=f"ROC AUC = {roc_auc:.2f}")
plt.plot([0, 1], [0, 1], color="navy", lw=2, linestyle="--")
plt.title("ROC-кривая")
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.legend(loc="lower right")

# PR-кривая
plt.subplot(1, 2, 2)
plt.plot(recall, precision, color="green", lw=2)
plt.title("PR-кривая (Precision-Recall)")
plt.xlabel("Recall")
plt.ylabel("Precision")

plt.tight_layout()
plt.show()
plt.show()
