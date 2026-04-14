#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score
from sklearn.preprocessing import StandardScaler

# 1. ЗАГРУЗКА И ПОДГОТОВКА ДАННЫХ
# Предполагается, что файл glass.csv находится в той же папке
# Если заголовков нет, укажем их вручную
columns = ['Id', 'RI', 'Na', 'Mg', 'Al', 'Si', 'K', 'Ca', 'Ba', 'Fe', 'Type']
data = pd.read_csv('glass.csv', names=columns) if 'Id' not in pd.read_csv('glass.csv', nrows=1).columns else pd.read_csv('glass.csv')

# Удаляем первый признак Id number (по условию)
data = data.drop(columns=['Id'])

X = data.drop('Type', axis=1)
y = data['Type']

# Масштабирование признаков (критично для k-NN, так как метод основан на расстояниях)
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.3, random_state=42)

# a. ПОСТРОЕНИЕ ГРАФИКА ЗАВИСИМОСТИ ОШИБКИ ОТ K
error_rates = []
k_range = range(1, 40)

for k in k_range:
    knn = KNeighborsClassifier(n_neighbors=k)
    knn.fit(X_train, y_train)
    pred_k = knn.predict(X_test)
    error_rates.append(np.mean(pred_k != y_test))

plt.figure(figsize=(10, 6))
plt.plot(k_range, error_rates, color='blue', linestyle='dashed', marker='o', markerfacecolor='red', markersize=8)
plt.title('Зависимость ошибки от количества соседей (K)')
plt.xlabel('K')
plt.ylabel('Доля ошибок (Error Rate)')
plt.grid(True)
plt.show()

# b. ИССЛЕДОВАНИЕ МЕТРИК РАССТОЯНИЯ
metrics = ['euclidean', 'manhattan', 'chebyshev']
print("Сравнение метрик (при k=5):")
for m in metrics:
    knn_m = KNeighborsClassifier(n_neighbors=5, metric=m)
    knn_m.fit(X_train, y_train)
    acc = accuracy_score(y_test, knn_m.predict(X_test))
    print(f"- {m.capitalize()}: Точность = {acc:.4f}")

# c. ПРЕДСКАЗАНИЕ ДЛЯ КОНКРЕТНОГО ЭКЗЕМПЛЯРА
sample = np.array([[1.516, 11.7, 1.01, 1.19, 72.59, 0.43, 11.44, 0.02, 0.1]])

# Используем модель с оптимальным k (например, k=5)
final_model = KNeighborsClassifier(n_neighbors=5, metric='manhattan')
final_model.fit(X_scaled, y) # Обучаем на всем наборе данных для лучшего прогноза
prediction = final_model.predict(sample)

print(f"\nРезультат для пункта (c):")
print(f"Экземпляр относится к типу стекла: {prediction[0]}")
