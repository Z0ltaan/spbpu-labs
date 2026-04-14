#!/usr/bin/env python3

# import numpy as np;
#
# def create_normal_distribution_arr(arr_size: int, ME, SD):
#   return np.random.normal(loc=ME, size=arr_size, scale=SD)
#
# if __name__ == "__main__":
#   point_count = 100
#   plus_count = 80
#   standard_deviation = 4
#   x1_minus = 22
#   x1_plus = 10
#
#   x2_minus = 5
#   x2_plus = 16
#
#   points_x1_minus = create_normal_distribution_arr(arr_size=point_count-plus_count, ME=x1_minus, SD=standard_deviation)
#   points_x1_plus = create_normal_distribution_arr(arr_size= plus_count, ME=x1_plus, SD=standard_deviation)
#
#   points_x2_minus = create_normal_distribution_arr(arr_size=point_count-plus_count, ME=x2_minus, SD=standard_deviation)
#   points_x2_plus = create_normal_distribution_arr(arr_size= plus_count, ME=x2_plus, SD=standard_deviation)


  ##########################
import numpy as np
import matplotlib.pyplot as plt
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score, confusion_matrix, roc_curve, auc, precision_recall_curve
from sklearn.model_selection import train_test_split

# 1. ГЕНЕРАЦИЯ ДАННЫХ ПО ВАШЕМУ ВАРИАНТУ
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

# Объединение в один датасет
X = np.vstack((X_minus1, X_plus1))
y = np.concatenate((y_minus1, y_plus1))

# 2. ВИЗУАЛИЗАЦИЯ (Диаграмма рассеяния)
plt.figure(figsize=(8, 6))
plt.scatter(X_minus1[:, 0], X_minus1[:, 1], color='red', label='Класс -1 (n=20)', edgecolors='k')
plt.scatter(X_plus1[:, 0], X_plus1[:, 1], color='blue', label='Класс 1 (n=80)', edgecolors='k')
plt.title("Распределение точек по варианту")
plt.xlabel("X1")
plt.ylabel("X2")
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.show()

# 3. ОБУЧЕНИЕ МОДЕЛИ
# Разделим на обучающую и тестовую выборки (70/30)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

model = GaussianNB()
model.fit(X_train, y_train)

# Прогноз
y_pred = model.predict(X_test)
y_probs = model.predict_proba(X_test)[:, 1] # Вероятности для класса 1

# 4. ВЫВОД МЕТРИК
print("--- Результаты классификации ---")
print(f"Accuracy (Точность): {accuracy_score(y_test, y_pred):.2f}")
print("Матрица ошибок (Confusion Matrix):")
print(confusion_matrix(y_test, y_pred))

# 5. ПОСТРОЕНИЕ КРИВЫХ (ROC и PR)
fpr, tpr, _ = roc_curve(y_test, y_probs)
roc_auc = auc(fpr, tpr)
precision, recall, _ = precision_recall_curve(y_test, y_probs)

plt.figure(figsize=(12, 5))

# ROC-кривая
plt.subplot(1, 2, 1)
plt.plot(fpr, tpr, color='darkorange', lw=2, label=f'ROC AUC = {roc_auc:.2f}')
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.title("ROC-кривая")
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.legend(loc="lower right")

# PR-кривая
plt.subplot(1, 2, 2)
plt.plot(recall, precision, color='green', lw=2)
plt.title("PR-кривая (Precision-Recall)")
plt.xlabel("Recall")
plt.ylabel("Precision")

plt.tight_layout()
plt.show()
plt.show()
