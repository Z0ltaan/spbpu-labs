#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score
from sklearn.preprocessing import LabelEncoder
import warnings

# warnings.filterwarnings('ignore')

# Функция для загрузки и подготовки данных
def load_and_prepare_data():
    tic_tac_toe_data = []
    with open('tic_tac_toe.txt', 'r') as f:
        for line in f:
            stripped_line = line.strip().split(',')
            tic_tac_toe_data.append(stripped_line)
            # print(stripped_line)
    tic_tac_toe_df = pd.DataFrame(tic_tac_toe_data)
    X_ttt = tic_tac_toe_df.iloc[:, :-1]
    y_ttt = tic_tac_toe_df.iloc[:, -1]

    # Кодирование категориальных признаков для tic-tac-toe
    le = LabelEncoder()
    for col in X_ttt.columns:
        X_ttt[col] = le.fit_transform(X_ttt[col])
    y_ttt = le.fit_transform(y_ttt)

    # Загрузка данных о спаме
    spam_df = pd.read_csv('spam.csv', encoding='latin-1')
    # Предполагаем, что последний столбец - целевой
    X_spam = spam_df.iloc[:, :-1].select_dtypes(include=[np.number])
    y_spam = spam_df.iloc[:, -1]
    if y_spam.dtype == 'object':
        y_spam = LabelEncoder().fit_transform(y_spam)

    return (X_ttt.to_numpy(), y_ttt), (X_spam.to_numpy(), y_spam)

# Функция для исследования зависимости точности от размера выборки
def investigate_sample_size_effect(X, y, test_sizes=[0.1, 0.2, 0.3, 0.4, 0.5]):
    train_accuracies = []
    test_accuracies = []

    for test_size in test_sizes:
        # Разделение данных
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=test_size, random_state=42
        )

        # Обучение модели
        gnb = GaussianNB()
        gnb.fit(X_train, y_train)

        # Оценка точности
        train_acc = accuracy_score(y_train, gnb.predict(X_train))
        test_acc = accuracy_score(y_test, gnb.predict(X_test))

        train_accuracies.append(train_acc)
        test_accuracies.append(test_acc)
        #
        # print(f"{dataset_name} - Test size {test_size:.0%}: Train Acc={train_acc:.3f}, Test Acc={test_acc:.3f}")

    return train_accuracies, test_accuracies
#
# Загрузка данных
print("Загрузка данных...")
(tic_tac_toe_X, tic_tac_toe_y), (spam_X, spam_y) = load_and_prepare_data()

# Исследование для tic-tac-toe
test_sizes = [0.1, 0.2, 0.3, 0.4, 0.5]
ttt_train_acc, ttt_test_acc = investigate_sample_size_effect(
    tic_tac_toe_X, tic_tac_toe_y, test_sizes
)

# Исследование для спама
spam_train_acc, spam_test_acc = investigate_sample_size_effect(
    spam_X, spam_y, test_sizes
)

print(len(spam_train_acc))

# Построение графиков
fig, axes = plt.subplots(1, 2, figsize=(14, 5), num='Task 1')
# fig.

# График для Tic-Tac-Toe
axes[0].plot(test_sizes, ttt_train_acc, 'o-', label='Обучающая выборка', linewidth=2)
axes[0].plot(test_sizes, ttt_test_acc, 's-', label='Тестовая выборка', linewidth=2)
axes[0].set_xlabel('Доля тестовой выборки')
axes[0].set_ylabel('Точность (Accuracy)')
axes[0].set_title('Tic-Tac-Toe: Зависимость точности от размера выборки')
axes[0].legend()
axes[0].grid(True, alpha=0.3)
axes[0].set_xticks(test_sizes)
axes[0].set_xticklabels([f'{int(s*100)}%' for s in test_sizes])

# График для Spam
axes[1].plot(test_sizes, spam_train_acc, 'o-', label='Обучающая выборка', linewidth=2)
axes[1].plot(test_sizes, spam_test_acc, 's-', label='Тестовая выборка', linewidth=2)
axes[1].set_xlabel('Доля тестовой выборки')
axes[1].set_ylabel('Точность (Accuracy)')
axes[1].set_title('Spam: Зависимость точности от размера выборки')
axes[1].legend()
axes[1].grid(True, alpha=0.3)
axes[1].set_xticks(test_sizes)
axes[1].set_xticklabels([f'{int(s*100)}%' for s in test_sizes])

# plt.tight_layout()
plt.show()
#
# # Анализ результатов
# # print("\n" + "="*50)
# # print("АНАЛИЗ РЕЗУЛЬТАТОВ")
# # print("="*50)
#
print("\nTic-Tac-Toe dataset:")
# print(f"Размер датасета: {len(tic_tac_toe_y)} образцов")
print(f"Средняя точность на обучающей выборке: {np.mean(ttt_train_acc):.3f}")
print(f"Средняя точность на тестовой выборке: {np.mean(ttt_test_acc):.3f}")
print(f"Разрыв между train и test: {np.mean(np.array(ttt_train_acc) - np.array(ttt_test_acc)):.3f}")

print("\nSpam dataset:")
# print(f"Размер датасета: {len(spam_y)} образцов")
print(f"Средняя точность на обучающей выборке: {np.mean(spam_train_acc):.3f}")
print(f"Средняя точность на тестовой выборке: {np.mean(spam_test_acc):.3f}")
print(f"Разрыв между train и test: {np.mean(np.array(spam_train_acc) - np.array(spam_test_acc)):.3f}")

