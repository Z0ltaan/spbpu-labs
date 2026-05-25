#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB, BernoulliNB, MultinomialNB
from sklearn.metrics import accuracy_score
from sklearn.preprocessing import LabelEncoder

def load_and_prepare_data():
    # --- Tic-Tac-Toe (используем BernoulliNB для бинарных признаков) ---
    tic_tac_toe_data = []
    with open("tic_tac_toe.txt", "r") as f:
        for line in f:
            if line.strip():
                tic_tac_toe_data.append(line.strip().split(","))
    tic_tac_toe_df = pd.DataFrame(tic_tac_toe_data)
    X_ttt = tic_tac_toe_df.iloc[:, :-1].copy()
    y_ttt = tic_tac_toe_df.iloc[:, -1]

    X_ttt = pd.get_dummies(X_ttt, drop_first=True).astype(float)
    y_ttt = LabelEncoder().fit_transform(y_ttt)

    # --- Spam (используем MultinomialNB для частот слов) ---
    spam_df = pd.read_csv("spam.csv")
    X_spam = spam_df.iloc[:, 1:-1].copy().astype(float)
    # Убираем отрицательные значения (если есть) для MultinomialNB
    X_spam = np.maximum(X_spam, 0)
    y_spam = spam_df.iloc[:, -1]
    y_spam = LabelEncoder().fit_transform(y_spam)

    return (X_ttt.to_numpy(), y_ttt), (X_spam.to_numpy(), y_spam)

def investigate_sample_size_effect_bernoulli(X, y, test_sizes):
    """Для Tic-Tac-Toe: BernoulliNB"""
    train_accuracies = []
    test_accuracies = []

    for test_size in test_sizes:
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=test_size, random_state=42
        )
        
        model = BernoulliNB(alpha=1.0)  # сглаживание Лапласа
        model.fit(X_train, y_train)
        
        train_acc = accuracy_score(y_train, model.predict(X_train))
        test_acc = accuracy_score(y_test, model.predict(X_test))
        
        train_accuracies.append(train_acc)
        test_accuracies.append(test_acc)
    
    return train_accuracies, test_accuracies

def investigate_sample_size_effect_multinomial(X, y, test_sizes):
    """Для Spam: MultinomialNB"""
    train_accuracies = []
    test_accuracies = []

    for test_size in test_sizes:
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=test_size, random_state=42
        )
        
        model = MultinomialNB(alpha=1.0)
        model.fit(X_train, y_train)
        
        train_acc = accuracy_score(y_train, model.predict(X_train))
        test_acc = accuracy_score(y_test, model.predict(X_test))
        
        train_accuracies.append(train_acc)
        test_accuracies.append(test_acc)
    
    return train_accuracies, test_accuracies

print("Загрузка и подготовка данных...")
(ttt_X, ttt_y), (spam_X, spam_y) = load_and_prepare_data()

test_sizes = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8]

print("Обработка Tic-Tac-Toe (BernoulliNB)...")
ttt_train, ttt_test = investigate_sample_size_effect_bernoulli(ttt_X, ttt_y, test_sizes)

print("Обработка Spam (MultinomialNB)...")
spam_train, spam_test = investigate_sample_size_effect_multinomial(spam_X, spam_y, test_sizes)

# Построение графиков
fig, axes = plt.subplots(1, 2, figsize=(14, 5))

# График для Tic-Tac-Toe
axes[0].plot(test_sizes, ttt_train, 'o-', color='tab:blue', label='Обучающая (Train)', linewidth=2, markersize=8)
axes[0].plot(test_sizes, ttt_test, 's-', color='tab:orange', label='Тестовая (Test)', linewidth=2, markersize=8)
axes[0].set_xlabel('Доля тестовой выборки', fontsize=12)
axes[0].set_ylabel('Точность (Accuracy)', fontsize=12)
axes[0].set_title('Tic-Tac-Toe: Зависимость от размера выборки', fontsize=14)
axes[0].legend(fontsize=11)
axes[0].grid(True, alpha=0.3)
# axes[0].set_xticks(test_sizes)
# axes[0].set_xticklabels([f'{int(s*100)}%' for s in test_sizes])
# axes[0].set_ylim([0.6, 1.0])  # Фиксируем диапазон для наглядности

# Добавляем подписи значений на графике Tic-Tac-Toe
for i, (x, y_train, y_test) in enumerate(zip(test_sizes, ttt_train, ttt_test)):
    axes[0].annotate(f'{y_train:.3f}', (x, y_train), textcoords="offset points", xytext=(0,10), ha='center', fontsize=8)
    axes[0].annotate(f'{y_test:.3f}', (x, y_test), textcoords="offset points", xytext=(0,-15), ha='center', fontsize=8)

# График для Spam
axes[1].plot(test_sizes, spam_train, 'o-', color='tab:blue', label='Обучающая (Train)', linewidth=2, markersize=8)
axes[1].plot(test_sizes, spam_test, 's-', color='tab:orange', label='Тестовая (Test)', linewidth=2, markersize=8)
axes[1].set_xlabel('Доля тестовой выборки', fontsize=12)
axes[1].set_ylabel('Точность (Accuracy)', fontsize=12)
axes[1].set_title('Spam: Зависимость от размера выборки', fontsize=14)
axes[1].legend(fontsize=11)
axes[1].grid(True, alpha=0.3)
# axes[1].set_xticks(test_sizes)
# axes[1].set_xticklabels([f'{int(s*100)}%' for s in test_sizes])
# axes[1].set_ylim([0.6, 1.0])

# Добавляем подписи значений на графике Spam
for i, (x, y_train, y_test) in enumerate(zip(test_sizes, spam_train, spam_test)):
    axes[1].annotate(f'{y_train:.3f}', (x, y_train), textcoords="offset points", xytext=(0,10), ha='center', fontsize=8)
    axes[1].annotate(f'{y_test:.3f}', (x, y_test), textcoords="offset points", xytext=(0,-15), ha='center', fontsize=8)

plt.tight_layout()
plt.show()

# Вывод результатов в таблицу
print("\n" + "="*60)
print("РЕЗУЛЬТАТЫ TIC-TAC-TOE (BernoulliNB):")
print("="*60)
print(f"{'Тест доля':<12} {'Train Acc':<12} {'Test Acc':<12}")
print("-"*36)
for ts, tr, te in zip(test_sizes, ttt_train, ttt_test):
    print(f"{int(ts*100):<12}% {tr:<12.4f} {te:<12.4f}")

print("\n" + "="*60)
print("РЕЗУЛЬТАТЫ SPAM (MultinomialNB):")
print("="*60)
print(f"{'Тест доля':<12} {'Train Acc':<12} {'Test Acc':<12}")
print("-"*36)
for ts, tr, te in zip(test_sizes, spam_train, spam_test):
    print(f"{int(ts*100):<12}% {tr:<12.4f} {te:<12.4f}")
