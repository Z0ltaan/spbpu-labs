#!/usr/bin/env python3

import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.tree import DecisionTreeRegressor
from sklearn.linear_model import LinearRegression
from sklearn.svm import SVR
from sklearn.metrics import r2_score, mean_squared_error

try:
    data = pd.read_csv('nsw74psid1.csv')
except FileNotFoundError:
    print("Файл nsw74psid1.csv не найден.")
    exit()

X = data.drop(columns=['re78'])
y = data['re78']

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

models = {
    'Регрессионное дерево': DecisionTreeRegressor(max_depth=5, random_state=42),
    'Линейная регрессия': LinearRegression(),
    'SVM регрессия (SVR)': SVR(kernel='rbf', C=1000, epsilon=0.1)
}

results = []

print("РАСЧЕТ МЕТРИК КАЧЕСТВА МОДЕЛЕЙ:\n" + "-" * 75)
print(f"{'Модель':<25} | {'R² Train':<10} | {'R² Test':<10} | {'MSE Test':<15}")
print("-" * 75)

for name, model in models.items():
    if name == 'Регрессионное дерево':
        model.fit(X_train, y_train)
        train_pred = model.predict(X_train)
        test_pred = model.predict(X_test)
    else:
        model.fit(X_train_scaled, y_train)
        train_pred = model.predict(X_train_scaled)
        test_pred = model.predict(X_test_scaled)
        
    r2_train = r2_score(y_train, train_pred)
    r2_test = r2_score(y_test, test_pred)
    mse_test = mean_squared_error(y_test, test_pred)
    
    results.append({'name': name, 'r2_test': r2_test, 'mse': mse_test})
    print(f"{name:<25} | {r2_train:<10.4f} | {r2_test:<10.4f} | {mse_test:<15.2f}")

print("-" * 75)

best_model = max(results, key=lambda x: x['r2_test'])
print(f"Оптимальная модель по критерию R²: {best_model['name']} (R² = {best_model['r2_test']:.4f})")
