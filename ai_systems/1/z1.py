#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score
from sklearn.preprocessing import LabelEncoder


def load_and_prepare_data():
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

    spam_df = pd.read_csv("spam.csv")
    X_spam = spam_df.iloc[:, 1:-1].copy().astype(float)
    y_spam = spam_df.iloc[:, -1]
    y_spam = LabelEncoder().fit_transform(y_spam)

    return (X_ttt.to_numpy(), y_ttt), (X_spam.to_numpy(), y_spam)


def investigate_sample_size_effect(X, y, test_sizes, var_smoothing=1e-9, n_runs=5):
    train_acc_mean = []
    test_acc_mean = []
    train_acc_std = []
    test_acc_std = []

    for test_size in test_sizes:
        train_accs = []
        test_accs = []

        X_train, X_test, y_train, y_test = train_test_split(
            X,
            y,
            test_size=test_size,
            random_state=42,
        )
        model = GaussianNB(var_smoothing=var_smoothing)
        model.fit(X_train, y_train)

        train_accs.append(accuracy_score(y_train, model.predict(X_train)))
        test_accs.append(accuracy_score(y_test, model.predict(X_test)))

        train_acc_mean.append(np.mean(train_accs))
        test_acc_mean.append(np.mean(test_accs))
        train_acc_std.append(np.std(train_accs))
        test_acc_std.append(np.std(test_accs))

    return train_acc_mean, test_acc_mean, train_acc_std, test_acc_std


print("Загрузка и подготовка данных")
(ttt_X, ttt_y), (spam_X, spam_y) = load_and_prepare_data()

test_sizes = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8]

ttt_train, ttt_test, ttt_train_std, ttt_test_std = investigate_sample_size_effect(
    ttt_X, ttt_y, test_sizes, var_smoothing=0.1, n_runs=5
)
spam_train, spam_test, spam_train_std, spam_test_std = investigate_sample_size_effect(
    spam_X, spam_y, test_sizes, var_smoothing=0.1, n_runs=5
)

fig, axes = plt.subplots(1, 2, figsize=(14, 5))

axes[0].plot(
    test_sizes,
    ttt_train,
    "o-",
    color="tab:blue",
    label="Обучающая (Train)",
    linewidth=2,
)
axes[0].plot(
    test_sizes, ttt_test, "s-", color="tab:orange", label="Тестовая (Test)", linewidth=2
)
axes[0].set_xlabel("Доля тестовой выборки")
axes[0].set_ylabel("Точность (Accuracy)")
axes[0].set_title("Tic-Tac-Toe: Зависимость от размера выборки")
axes[0].legend()
axes[0].grid(True, alpha=0.3)
axes[0].set_xticks(test_sizes)
axes[0].set_xticklabels([f"{int(s * 100)}%" for s in test_sizes])

# График для Spam
axes[1].plot(
    test_sizes,
    spam_train,
    "o-",
    color="tab:blue",
    label="Обучающая (Train)",
    linewidth=2,
)
axes[1].plot(
    test_sizes,
    spam_test,
    "s-",
    color="tab:orange",
    label="Тестовая (Test)",
    linewidth=2,
)
axes[1].set_xlabel("Доля тестовой выборки")
axes[1].set_ylabel("Точность (Accuracy)")
axes[1].set_title("Spam: Зависимость от размера выборки")
axes[1].legend()
axes[1].grid(True, alpha=0.3)
plt.show()
