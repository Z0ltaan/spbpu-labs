#!/usr/bin/env python3

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from matplotlib.colors import ListedColormap
from sklearn import svm
from sklearn.metrics import confusion_matrix, accuracy_score
import warnings

warnings.filterwarnings("ignore")

plt.rcParams["figure.figsize"] = [12, 8]
plt.rcParams["font.size"] = 12


def load_svm_data(file_path):
    df = pd.read_csv(file_path, sep="\t", skipinitialspace=True)
    if "Colors" in df.columns:
        target_col = "Colors"
    elif "Color" in df.columns:
        target_col = "Color"
    else:
        raise ValueError(f"Не найден столбец с метками класса в {file_path}")

    X = df[["X1", "X2"]].values
    y = df[target_col].values
    y = np.array([0 if label == "red" else 1 for label in y])
    return X, y, df


def plot_decision_boundary(X, y, clf, title, ax=None, show_support_vectors=False):
    if ax is None:
        fig, ax = plt.subplots(1, 1, figsize=(10, 8))

    h = 0.02  # шаг сетки
    x_min, x_max = X[:, 0].min() - 0.5, X[:, 0].max() + 0.5
    y_min, y_max = X[:, 1].min() - 0.5, X[:, 1].max() + 0.5
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))

    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)

    cmap_light = ListedColormap(["#FFAAAA", "#AAFFAA"])
    cmap_bold = ListedColormap(["#FF0000", "#00FF00"])

    ax.contourf(xx, yy, Z, cmap=cmap_light, alpha=0.3)

    ax.contour(xx, yy, Z, colors="black", linewidths=1.5)

    scatter = ax.scatter(
        X[:, 0], X[:, 1], c=y, cmap=cmap_bold, edgecolor="black", s=50, alpha=0.8
    )

    if show_support_vectors and hasattr(clf, "support_vectors_"):
        ax.scatter(
            clf.support_vectors_[:, 0],
            clf.support_vectors_[:, 1],
            s=200,
            linewidth=1.5,
            facecolors="none",
            edgecolors="blue",
            label="Опорные векторы",
        )

    ax.set_xlabel("X1")
    ax.set_ylabel("X2")
    ax.set_title(title)
    ax.legend()
    ax.grid(True, alpha=0.3)

    return ax


def evaluate_model(clf, X_train, y_train, X_test, y_test, dataset_name):
    print(f"\n=== Результаты для {dataset_name} ===")

    y_train_pred = clf.predict(X_train)
    y_test_pred = clf.predict(X_test)

    cm_train = confusion_matrix(y_train, y_train_pred)
    cm_test = confusion_matrix(y_test, y_test_pred)

    train_acc = accuracy_score(y_train, y_train_pred)
    test_acc = accuracy_score(y_test, y_test_pred)

    print(f"Количество опорных векторов: {len(clf.support_vectors_)}")
    print(f"Точность на обучающей выборке: {train_acc:.4f}")
    print(f"Точность на тестовой выборке: {test_acc:.4f}")

    print("\nМатрица ошибок (обучающая выборка):")
    print(cm_train)
    print("\nМатрица ошибок (тестовая выборка):")
    print(cm_test)

    return train_acc, test_acc


# ============================================================
# ЗАГРУЗКА И ОБУЧЕНИЕ МОДЕЛЕЙ
# ============================================================
X_train_a, y_train_a, _ = load_svm_data("svmdata_a.txt")
X_test_a, y_test_a, _ = load_svm_data("svmdata_a_test.txt")

X_train_b, y_train_b, _ = load_svm_data("svmdata_b.txt")
X_test_b, y_test_b, _ = load_svm_data("svmdata_b_test.txt")

# Модель А
clf_a_linear = svm.SVC(kernel="linear", C=1.0, random_state=42)
clf_a_linear.fit(X_train_a, y_train_a)

# Модель Б
clf_b_linear = svm.SVC(kernel="linear", C=1.0, random_state=42)
clf_b_linear.fit(X_train_b, y_train_b)

print("=" * 60)
print("ВЫЧИСЛЕНИЯ И МАТРИЦЫ ОШИБОК")
print("=" * 60)
evaluate_model(clf_a_linear, X_train_a, y_train_a, X_test_a, y_test_a, "svmdata_a")
evaluate_model(clf_b_linear, X_train_b, y_train_b, X_test_b, y_test_b, "svmdata_b")


# ============================================================
# ГРАФИК 1: ЗАДАНИЕ 4a — Только разбиение пространства на области
# ============================================================
fig_a, axes_a = plt.subplots(2, 2, figsize=(16, 12))

plot_decision_boundary(
    X_train_a,
    y_train_a,
    clf_a_linear,
    "svmdata_a (обучающая) - Области",
    ax=axes_a[0, 0],
    show_support_vectors=False,
)

plot_decision_boundary(
    X_test_a,
    y_test_a,
    clf_a_linear,
    "svmdata_a (тестовая) - Области",
    ax=axes_a[0, 1],
    show_support_vectors=False,
)

plot_decision_boundary(
    X_train_b,
    y_train_b,
    clf_b_linear,
    "svmdata_b (обучающая) - Области",
    ax=axes_a[1, 0],
    show_support_vectors=False,
)

plot_decision_boundary(
    X_test_b,
    y_test_b,
    clf_b_linear,
    "svmdata_b (тестовая) - Области",
    ax=axes_a[1, 1],
    show_support_vectors=False,
)

fig_a.suptitle(
    "Задание 4a: Разбиение пространства признаков на области",
    fontsize=16,
    fontweight="bold",
)
plt.tight_layout()
plt.show()

# ============================================================
# ГРАФИК 2: ЗАДАНИЕ 4b — Визуализация опорных векторов на фоне областей
# ============================================================
fig_b, axes_b = plt.subplots(2, 2, figsize=(16, 12))

plot_decision_boundary(
    X_train_a,
    y_train_a,
    clf_a_linear,
    "svmdata_a (обучающая) + Опорные вектора",
    ax=axes_b[0, 0],
    show_support_vectors=True,
)

plot_decision_boundary(
    X_test_a,
    y_test_a,
    clf_a_linear,
    "svmdata_a (тестовая) - Без векторов",
    ax=axes_b[0, 1],
    show_support_vectors=False,
)

plot_decision_boundary(
    X_train_b,
    y_train_b,
    clf_b_linear,
    "svmdata_b (обучающая) + Опорные вектора",
    ax=axes_b[1, 0],
    show_support_vectors=True,
)

plot_decision_boundary(
    X_test_b,
    y_test_b,
    clf_b_linear,
    "svmdata_b (тестовая) - Без векторов",
    ax=axes_b[1, 1],
    show_support_vectors=False,
)

fig_b.suptitle(
    "Задание 4b: Визуализация опорных векторов на фоне разбиения",
    fontsize=16,
    fontweight="bold",
)
plt.tight_layout()
plt.show()

# ============================================
# ЗАДАНИЕ 4c: Подбор параметра C для нулевой ошибки
# ============================================
print("\n" + "=" * 60)
print("ЗАДАНИЕ 4c: Подбор параметра C для нулевой ошибки")
print("=" * 60)

print("\n--- Загрузка данных svmdata_c ---")
try:
    X_train_c, y_train_c, df_train_c = load_svm_data("svmdata_c.txt")
    X_test_c, y_test_c, df_test_c = load_svm_data("svmdata_c_test.txt")

    print(f"Обучающая выборка: {X_train_c.shape[0]} образцов")
    print(f"Тестовая выборка: {X_test_c.shape[0]} образцов")
    print(
        f"Соотношение классов в обучении: {np.sum(y_train_c == 0)} red, {np.sum(y_train_c == 1)} green"
    )
    print(
        f"Соотношение классов в тесте: {np.sum(y_test_c == 0)} red, {np.sum(y_test_c == 1)} green"
    )

    # Поиск оптимального C
    C_values = [
        0.00001,
        0.0001,
        0.001,
        0.01,
        0.1,
        1,
        10,
        100,
        1000,
        2500,
        10000,
        1000000,
        5000000
    ]  # 100000, 1000000]
    results = []

    print("\n--- Поиск оптимального параметра C ---")
    print(
        f"{'C':>15} | {'Train acc':>10} | {'Test acc':>10} | {'Train errors':>12} | {'Test errors':>12} | {'SV count':>10}"
    )
    print("-" * 75)

    for C in C_values:
        svm_c = svm.SVC(kernel="linear", C=C, random_state=42)
        svm_c.fit(X_train_c, y_train_c)

        train_pred = svm_c.predict(X_train_c)
        test_pred = svm_c.predict(X_test_c)

        train_acc = accuracy_score(y_train_c, train_pred)
        test_acc = accuracy_score(y_test_c, test_pred)
        train_errors = np.sum(train_pred != y_train_c)
        test_errors = np.sum(test_pred != y_test_c)

        results.append(
            {
                "C": C,
                "svm": svm_c,
                "train_acc": train_acc,
                "test_acc": test_acc,
                "train_errors": train_errors,
                "test_errors": test_errors,
                "sv_count": len(svm_c.support_vectors_),
            }
        )

        print(
            f"{C:15.5f} | {train_acc:10.4f} | {test_acc:10.4f} | {train_errors:12} | {test_errors:12} | {svm_c.support_vectors_.shape[0]:10}"
        )

    zero_train = [r for r in results if r["train_errors"] == 0]

    if zero_train:
        print("\n--- Параметры C с нулевой ошибкой на обучении ---")
        for r in zero_train:
            print(
                f"C = {r['C']:.3f}: test_acc = {r['test_acc']:.4f}, SV = {r['sv_count']}"
            )

        # Выбираем оптимальный C (максимальная точность на тесте среди нулевой ошибки на обучении)
        best = max(zero_train, key=lambda x: x["test_acc"])
        print(f"\nОптимальный C = {best['C']:.3f}")
        print(f"  - Точность на обучении: {best['train_acc']:.4f} (0 ошибок)")
        print(f"  - Точность на тесте: {best['test_acc']:.4f}")
        print(f"  - Количество опорных векторов: {best['sv_count']}")

        clf_c_best = best["svm"]
    else:
        # Если нулевой ошибки нет, берем максимальную точность на тесте
        best = max(results, key=lambda x: x["test_acc"])
        print(f"\nНулевая ошибка на обучении не достигнута")
        print(f"Лучший C = {best['C']:.5f} (по точности на тесте)")
        clf_c_best = best["svm"]

    fig, axes = plt.subplots(1, 2, figsize=(15, 5))

    # График точности от C
    axes[0].semilogx(
        [r["C"] for r in results],
        [r["train_acc"] for r in results],
        "o-",
        label="Train",
        linewidth=2,
    )
    axes[0].semilogx(
        [r["C"] for r in results],
        [r["test_acc"] for r in results],
        "s-",
        label="Test",
        linewidth=2,
    )
    axes[0].axhline(y=1.0, color="g", linestyle="--", alpha=0.5, label="Zero error")
    axes[0].set_xlabel("C (log scale)")
    axes[0].set_ylabel("Accuracy")
    axes[0].set_title("Влияние параметра C на точность")
    axes[0].legend()
    axes[0].grid(True, alpha=0.3)

    # График количества опорных векторов
    axes[1].semilogx(
        [r["C"] for r in results],
        [r["sv_count"] for r in results],
        "o-",
        color="orange",
        linewidth=2,
    )
    axes[1].set_xlabel("C (log scale)")
    axes[1].set_ylabel("Number of Support Vectors")
    axes[1].set_title("Количество опорных векторов от C")
    axes[1].grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig("svm_C_optimization.png", dpi=150)
    plt.show()

except FileNotFoundError:
    print("Файлы svmdata_c.txt или svmdata_c_test.txt не найдены")


# ============================================
# ЗАДАНИЕ 4d: Различные ядра
# ============================================
print("\n" + "="*60)
print("ЗАДАНИE 4d: Сравнение различных ядер")
print("="*60)

X_demo, y_demo, df = load_svm_data("svmdata_d.txt")
X_test_demo, y_test_demo, df_test = load_svm_data("svmdata_d_test.txt")

kernels = {
    'Linear (линейное)': {'kernel': 'linear', 'params': {}},
    'Poly degree 1': {'kernel': 'poly', 'params': {'degree': 1, 'coef0': 1}},
    'Poly degree 2': {'kernel': 'poly', 'params': {'degree': 2, 'coef0': 1}},
    'Poly degree 3': {'kernel': 'poly', 'params': {'degree': 3, 'coef0': 1}},
    'Poly degree 4': {'kernel': 'poly', 'params': {'degree': 4, 'coef0': 1}},
    'Poly degree 5': {'kernel': 'poly', 'params': {'degree': 5, 'coef0': 1}},
    'RBF': {'kernel': 'rbf', 'params': {'gamma': 'scale'}},
    'Sigmoid': {'kernel': 'sigmoid', 'params': {'gamma': 'scale', 'coef0': 1}}
}

C_fixed = 1.0

models = {}
results_kernels = []

print(f"\nСравнение ядер (C={C_fixed}):")
print(f"{'Ядро':<20} | {'Train acc':>10} | {'Test acc':>10} | {'SV count':>10}")
print("-" * 55)

for name, kernel_info in kernels.items():
    svm_model = svm.SVC(kernel=kernel_info['kernel'], C=C_fixed,
                        random_state=42, **kernel_info['params'])
    svm_model.fit(X_demo, y_demo)

    train_acc = accuracy_score(y_demo, svm_model.predict(X_demo))
    test_acc = accuracy_score(y_test_demo, svm_model.predict(X_test_demo))
    sv_count = len(svm_model.support_vectors_)

    models[name] = svm_model
    results_kernels.append({
        'name': name,
        'train_acc': train_acc,
        'test_acc': test_acc,
        'sv_count': sv_count
    })

    print(f"{name:<20} | {train_acc:10.4f} | {test_acc:10.4f} | {sv_count:10}")

n_kernels = len(kernels)
n_cols = 4
n_rows = (n_kernels + n_cols - 1) // n_cols

fig, axes = plt.subplots(n_rows, n_cols, figsize=(20, 5*n_rows))
axes = axes.flatten()

for idx, (name, svm_model) in enumerate(models.items()):
    plot_decision_boundary(X_demo, y_demo, svm_model,
                          f"{name}\nTrain acc: {results_kernels[idx]['train_acc']:.3f}, Test acc: {results_kernels[idx]['test_acc']:.3f}",
                          ax=axes[idx], show_support_vectors=True)

for idx in range(len(kernels), len(axes)):
    axes[idx].axis('off')

plt.suptitle('Сравнение различных ядер SVM (обучающая выборка)', fontsize=16, y=1.02)
plt.tight_layout()
plt.show()


# ============================================
# ЗАДАНИЕ 4d: Различные ядра
# ============================================
print("\n" + "="*60)
print("ЗАДАНИE 4e: Сравнение различных ядер")
print("="*60)

X_demo, y_demo, df = load_svm_data("svmdata_e.txt")
X_test_demo, y_test_demo, df_test = load_svm_data("svmdata_e_test.txt")

kernels = {
    'Linear (линейное)': {'kernel': 'linear', 'params': {}},
    'Poly degree 1': {'kernel': 'poly', 'params': {'degree': 1, 'coef0': 1}},
    'Poly degree 2': {'kernel': 'poly', 'params': {'degree': 2, 'coef0': 1}},
    'Poly degree 3': {'kernel': 'poly', 'params': {'degree': 3, 'coef0': 1}},
    'Poly degree 4': {'kernel': 'poly', 'params': {'degree': 4, 'coef0': 1}},
    'Poly degree 5': {'kernel': 'poly', 'params': {'degree': 5, 'coef0': 1}},
    'RBF': {'kernel': 'rbf', 'params': {'gamma': 'scale'}},
    'Sigmoid': {'kernel': 'sigmoid', 'params': {'gamma': 'scale', 'coef0': 1}}
}

C_fixed = 1.0

models = {}
results_kernels = []

print(f"\nСравнение ядер (C={C_fixed}):")
print(f"{'Ядро':<20} | {'Train acc':>10} | {'Test acc':>10} | {'SV count':>10}")
print("-" * 55)

for name, kernel_info in kernels.items():
    svm_model = svm.SVC(kernel=kernel_info['kernel'], C=C_fixed,
                        random_state=42, **kernel_info['params'])
    svm_model.fit(X_demo, y_demo)

    train_acc = accuracy_score(y_demo, svm_model.predict(X_demo))
    test_acc = accuracy_score(y_test_demo, svm_model.predict(X_test_demo))
    sv_count = len(svm_model.support_vectors_)

    models[name] = svm_model
    results_kernels.append({
        'name': name,
        'train_acc': train_acc,
        'test_acc': test_acc,
        'sv_count': sv_count
    })

    print(f"{name:<20} | {train_acc:10.4f} | {test_acc:10.4f} | {sv_count:10}")

# Визуализация границ для всех ядер
n_kernels = len(kernels)
n_cols = 4
n_rows = (n_kernels + n_cols - 1) // n_cols

fig, axes = plt.subplots(n_rows, n_cols, figsize=(20, 5*n_rows))
axes = axes.flatten()

for idx, (name, svm_model) in enumerate(models.items()):
    plot_decision_boundary(X_demo, y_demo, svm_model,
                          f"{name}\nTrain acc: {results_kernels[idx]['train_acc']:.3f}, Test acc: {results_kernels[idx]['test_acc']:.3f}",
                          ax=axes[idx], show_support_vectors=True)

for idx in range(len(kernels), len(axes)):
    axes[idx].axis('off')

plt.suptitle('Сравнение различных ядер SVM (обучающая выборка)', fontsize=16, y=1.02)
plt.tight_layout()
plt.show()



# ============================================
# ЗАДАНИЕ 4f: Эффект переобучения (изменение gamma)
# ============================================
X_demo, y_demo, df = load_svm_data("svmdata_e.txt")
X_test_demo, y_test_demo, df_test = load_svm_data("svmdata_e_test.txt")


print("\n" + "="*60)
print("ЗАДАНИЕ 4f: Эффект переобучения (изменение gamma для RBF ядра)")
print("="*60)

gamma_values = [0.001, 0.01, 0.1, 0.5, 1, 2, 5, 10, 50, 100]
C_fixed = 1.0

print(f"\nИсследование влияния gamma (C={C_fixed}):")
print(f"{'gamma':>10} | {'Train acc':>10} | {'Test acc':>10} | {'SV count':>10} | {'Status':>15}")
print("-" * 65)

gamma_models = []
gamma_results = []

for gamma in gamma_values:
    svm_gamma = svm.SVC(kernel='rbf', gamma=gamma, C=C_fixed, random_state=42)
    svm_gamma.fit(X_demo, y_demo)

    train_acc = accuracy_score(y_demo, svm_gamma.predict(X_demo))
    test_acc = accuracy_score(y_test_demo, svm_gamma.predict(X_test_demo))
    sv_count = len(svm_gamma.support_vectors_)

    # Определяем состояние модели
    if train_acc > 0.99 and test_acc < train_acc - 0.05:
        status = "ПЕРЕОБУЧЕНИЕ"
    elif train_acc < 0.8:
        status = "НЕДООБУЧЕНИЕ"
    else:
        status = "ХОРОШО"

    gamma_results.append({
        'gamma': gamma,
        'train_acc': train_acc,
        'test_acc': test_acc,
        'sv_count': sv_count,
        'status': status
    })
    gamma_models.append(svm_gamma)

    print(f"{gamma:10.3f} | {train_acc:10.4f} | {test_acc:10.4f} | {sv_count:10} | {status:>15}")

# Визуализация влияния gamma
fig, axes = plt.subplots(2, 5, figsize=(20, 8))
axes = axes.flatten()

for idx, (gamma, svm_model) in enumerate(zip(gamma_values, gamma_models)):
    plot_decision_boundary(X_demo, y_demo, svm_model,
                          f"gamma={gamma}\nTrain: {gamma_results[idx]['train_acc']:.3f}, Test: {gamma_results[idx]['test_acc']:.3f}",
                          ax=axes[idx], show_support_vectors=True)
    if gamma_results[idx]['status'] == "ПЕРЕОБУЧЕНИЕ":
        axes[idx].set_facecolor('#FFEEEE')

plt.suptitle('Влияние параметра gamma RBF ядра на переобучение', fontsize=16, y=1.02)
plt.tight_layout()
plt.savefig('svm_gamma_effect.png', dpi=150, bbox_inches='tight')
plt.show()

# Графики зависимости точности от gamma
fig, axes = plt.subplots(1, 2, figsize=(15, 6))

# График точности
axes[0].semilogx(gamma_values, [r['train_acc'] for r in gamma_results], 'o-', label='Train', linewidth=2, markersize=8)
axes[0].semilogx(gamma_values, [r['test_acc'] for r in gamma_results], 's-', label='Test', linewidth=2, markersize=8)
axes[0].set_xlabel('gamma (log scale)')
axes[0].set_ylabel('Accuracy')
axes[0].set_title('Зависимость точности от параметра gamma')
axes[0].legend()
axes[0].grid(True, alpha=0.3)

# Отмечаем области
axes[0].axvline(x=0.1, color='green', linestyle='--', alpha=0.5, label='Хорошая область')
axes[0].axvline(x=5, color='orange', linestyle='--', alpha=0.5, label='Начало переобучения')
axes[0].legend()

# График количества опорных векторов
axes[1].semilogx(gamma_values, [r['sv_count'] for r in gamma_results], 'o-', color='purple', linewidth=2, markersize=8)
axes[1].set_xlabel('gamma (log scale)')
axes[1].set_ylabel('Number of Support Vectors')
axes[1].set_title('Зависимость количества опорных векторов от gamma')
axes[1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('svm_gamma_analysis.png', dpi=150)
plt.show()

print("\n--- Детальный анализ переобучения ---")

np.random.seed(42)
X_complex = np.random.randn(100, 2) * 1.5
y_complex = np.zeros(100)
for i in range(100):
    if X_complex[i, 0]**2 + X_complex[i, 1]**2 < 1:
        y_complex[i] = 1

X_complex[:20] += np.random.randn(20, 2) * 0.5

# Обучаем с разными gamma
fig, axes = plt.subplots(2, 3, figsize=(18, 12))
axes = axes.flatten()

test_gammas = [0.01, 0.1, 1, 5, 20, 100]

for idx, gamma in enumerate(test_gammas):
    svm_test = svm.SVC(kernel='rbf', gamma=gamma, C=1, random_state=42)
    svm_test.fit(X_complex, y_complex)

    plot_decision_boundary(X_complex, y_complex, svm_test,
                          f"gamma={gamma}\nSV={len(svm_test.support_vectors_)}",
                          ax=axes[idx], show_support_vectors=True)

plt.suptitle('Демонстрация переобучения RBF ядра при увеличении gamma', fontsize=16, y=1.02)
plt.tight_layout()
plt.savefig('svm_overfitting_demo.png', dpi=150, bbox_inches='tight')
plt.show()
