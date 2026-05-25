#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression, Ridge
from sklearn.metrics import mean_squared_error

try:
    data = pd.read_csv('longley.csv')
except FileNotFoundError:
    print("Файл longley.csv не найден.")
    exit()

data = data.drop(columns=['Population'])

X = data.drop(columns=['Employed'])
y = data['Employed']
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=42)
ols = LinearRegression()
ols.fit(X_train, y_train)

ols_train_mse = mean_squared_error(y_train, ols.predict(X_train))
ols_test_mse = mean_squared_error(y_test, ols.predict(X_test))

i_values = np.arange(26)
lambdas = 10 ** (-3 + 0.2 * i_values)

ridge_train_errors = []
ridge_test_errors = []

for lmbda in lambdas:
    ridge = Ridge(alpha=lmbda)
    ridge.fit(X_train, y_train)
    
    train_mse = mean_squared_error(y_train, ridge.predict(X_train))
    test_mse = mean_squared_error(y_test, ridge.predict(X_test))
    
    ridge_train_errors.append(train_mse)
    ridge_test_errors.append(test_mse)

plt.figure(figsize=(10, 6))
plt.grid(True, linestyle='--', alpha=0.6)

plt.plot(lambdas, ridge_train_errors, color='blue', label='Ridge (Обучающая)', linewidth=2)
plt.plot(lambdas, ridge_test_errors, color='red', label='Ridge (Тестовая)', linewidth=2)

plt.axhline(y=ols_train_mse, color='blue', linestyle='--', alpha=0.7, label='OLS (Обучающая)')
plt.axhline(y=ols_test_mse, color='red', linestyle='--', alpha=0.7, label='OLS (Тестовая)')

plt.xscale('log')

plt.title('Зависимость ошибки (MSE) от параметра регуляризации $\lambda$', fontsize=12, fontweight='bold')
plt.xlabel('Параметр регуляризации $\lambda$ (log scale)', fontsize=11)
plt.ylabel('Среднеквадратичная ошибка (MSE)', fontsize=11)
plt.legend(frameon=True, facecolor='white')

plt.tight_layout()
plt.savefig('ridge_vs_ols.png', dpi=300)
plt.show()

print("РЕЗУЛЬТАТЫ:")
print(f"OLS Train MSE: {ols_train_mse:.4f} | OLS Test MSE: {ols_test_mse:.4f}")
best_idx = np.argmin(ridge_test_errors)
print(f"Лучший Ridge: {lambdas[best_idx]:.4f}")
print(f"Ridge Train MSE при лучшемb: {ridge_train_errors[best_idx]:.4f}")
print(f"Ridge Test MSE при лучшемb: {ridge_test_errors[best_idx]:.4f}")
