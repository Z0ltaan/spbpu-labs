#!/usr/bin/env python3

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from matplotlib.colors import ListedColormap
from sklearn import svm
from sklearn.metrics import confusion_matrix, accuracy_score
import warnings
warnings.filterwarnings('ignore')

# Настройка отображения графиков
plt.rcParams['figure.figsize'] = [12, 8]
plt.rcParams['font.size'] = 12

# Функция для загрузки данных
def load_svm_data(file_path):
    df = pd.read_csv(file_path, sep='\t', skipinitialspace=True)
    # Проверяем название столбца с метками класса
    if 'Colors' in df.columns:
        target_col = 'Colors'
    elif 'Color' in df.columns:
        target_col = 'Color'
    else:
        raise ValueError(f"Не найден столбец с метками класса в {file_path}")
    
    X = df[['X1', 'X2']].values
    y = df[target_col].values
    # Преобразуем метки в числовые значения: red -> 0, green -> 1
    y = np.array([0 if label == 'red' else 1 for label in y])
    return X, y, df

# Функция для визуализации разделения пространства признаков
def plot_decision_boundary(X, y, clf, title, ax=None, show_support_vectors=True):
    if ax is None:
        fig, ax = plt.subplots(1, 1, figsize=(10, 8))
    
    # Создаем сетку для визуализации
    h = 0.02  # шаг сетки
    x_min, x_max = X[:, 0].min() - 0.5, X[:, 0].max() + 0.5
    y_min, y_max = X[:, 1].min() - 0.5, X[:, 1].max() + 0.5
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))
    
    # Предсказания для каждой точки сетки
    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    
    # Цветовая карта для фона
    cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA'])
    cmap_bold = ListedColormap(['#FF0000', '#00FF00'])
    
    # Рисуем контурную карту
    ax.contourf(xx, yy, Z, cmap=cmap_light, alpha=0.3)
    
    # Рисуем границу решения
    ax.contour(xx, yy, Z, colors='black', linewidths=0.5)
    
    # Рисуем точки данных
    scatter = ax.scatter(X[:, 0], X[:, 1], c=y, cmap=cmap_bold, 
                         edgecolor='black', s=50, alpha=0.8)
    
    # Рисуем опорные векторы
    if show_support_vectors and hasattr(clf, 'support_vectors_'):
        ax.scatter(clf.support_vectors_[:, 0], clf.support_vectors_[:, 1], 
                   s=200, linewidth=1, facecolors='none', edgecolors='blue', 
                   label='Опорные векторы')
    
    ax.set_xlabel('X1')
    ax.set_ylabel('X2')
    ax.set_title(title)
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    return ax

# Функция для оценки модели
def evaluate_model(clf, X_train, y_train, X_test, y_test, dataset_name):
    print(f"\n=== Результаты для {dataset_name} ===")
    
    # Предсказания
    y_train_pred = clf.predict(X_train)
    y_test_pred = clf.predict(X_test)
    
    # Матрицы ошибок
    cm_train = confusion_matrix(y_train, y_train_pred)
    cm_test = confusion_matrix(y_test, y_test_pred)
    
    # Точность
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

# Загрузка данных для svmdata_a
print("="*60)
print("АНАЛИЗ ДАННЫХ svmdata_a")
print("="*60)

X_train_a, y_train_a, df_train_a = load_svm_data('svmdata_a.txt')
X_test_a, y_test_a, df_test_a = load_svm_data('svmdata_a_test.txt')

print(f"Обучающая выборка: {X_train_a.shape[0]} образцов")
print(f"Тестовая выборка: {X_test_a.shape[0]} образцов")
print(f"Классы: 0 (red), 1 (green)")

# Загрузка данных для svmdata_b
print("\n" + "="*60)
print("АНАЛИЗ ДАННЫХ svmdata_b")
print("="*60)

X_train_b, y_train_b, df_train_b = load_svm_data('svmdata_b.txt')
X_test_b, y_test_b, df_test_b = load_svm_data('svmdata_b_test.txt')

print(f"Обучающая выборка: {X_train_b.shape[0]} образцов")
print(f"Тестовая выборка: {X_test_b.shape[0]} образцов")
print(f"Классы: 0 (red), 1 (green)")

# === ЗАДАНИЕ 4a: Линейное ядро ===
print("\n" + "="*60)
print("ЗАДАНИЕ 4a: Линейное ядро")
print("="*60)

# SVM с линейным ядром для svmdata_a
clf_a_linear = svm.SVC(kernel='linear', C=1.0, random_state=42)
clf_a_linear.fit(X_train_a, y_train_a)

print("\n--- svmdata_a ---")
train_acc_a, test_acc_a = evaluate_model(clf_a_linear, X_train_a, y_train_a, 
                                         X_test_a, y_test_a, "svmdata_a (линейное ядро)")

# SVM с линейным ядром для svmdata_b
clf_b_linear = svm.SVC(kernel='linear', C=1.0, random_state=42)
clf_b_linear.fit(X_train_b, y_train_b)

print("\n--- svmdata_b ---")
train_acc_b, test_acc_b = evaluate_model(clf_b_linear, X_train_b, y_train_b, 
                                         X_test_b, y_test_b, "svmdata_b (линейное ядро)")

# === ЗАДАНИЕ 4b: Визуализация опорных векторов ===
print("\n" + "="*60)
print("ЗАДАНИЕ 4b: Визуализация опорных векторов")
print("="*60)

fig, axes = plt.subplots(2, 2, figsize=(16, 12))

# svmdata_a - обучающая выборка
plot_decision_boundary(X_train_a, y_train_a, clf_a_linear, 
                      "svmdata_a (обучающая выборка) - линейное ядро", 
                      ax=axes[0, 0], show_support_vectors=True)

# svmdata_a - тестовая выборка
plot_decision_boundary(X_test_a, y_test_a, clf_a_linear, 
                      "svmdata_a (тестовая выборка) - линейное ядро", 
                      ax=axes[0, 1], show_support_vectors=True)

# svmdata_b - обучающая выборка
plot_decision_boundary(X_train_b, y_train_b, clf_b_linear, 
                      "svmdata_b (обучающая выборка) - линейное ядро", 
                      ax=axes[1, 0], show_support_vectors=True)

# svmdata_b - тестовая выборка
plot_decision_boundary(X_test_b, y_test_b, clf_b_linear, 
                      "svmdata_b (тестовая выборка) - линейное ядро", 
                      ax=axes[1, 1], show_support_vectors=True)

plt.tight_layout()
plt.savefig('svm_linear_boundaries.png', dpi=150)
plt.show()


# ============================================
# ЗАДАНИЕ 4c: Подбор параметра C для нулевой ошибки
# ============================================
print("\n" + "="*60)
print("ЗАДАНИЕ 4c: Подбор параметра C для нулевой ошибки")
print("="*60)

# Загрузка данных для svmdata_c
print("\n--- Загрузка данных svmdata_c ---")
try:
    X_train_c, y_train_c, df_train_c = load_svm_data('svmdata_c.txt')
    X_test_c, y_test_c, df_test_c = load_svm_data('svmdata_c_test.txt')
    
    print(f"Обучающая выборка: {X_train_c.shape[0]} образцов")
    print(f"Тестовая выборка: {X_test_c.shape[0]} образцов")
    print(f"Соотношение классов в обучении: {np.sum(y_train_c==0)} red, {np.sum(y_train_c==1)} green")
    print(f"Соотношение классов в тесте: {np.sum(y_test_c==0)} red, {np.sum(y_test_c==1)} green")
    
    # Поиск оптимального C
    C_values = [0.001, 0.01, 0.1, 0.5, 1, 5, 10, 50, 100, 500, 1000]
    results = []
    
    print("\n--- Поиск оптимального параметра C ---")
    print(f"{'C':>10} | {'Train acc':>10} | {'Test acc':>10} | {'Train errors':>12} | {'Test errors':>12} | {'SV count':>10}")
    print("-" * 75)
    
    for C in C_values:
        svm_c = svm.SVC(kernel='linear', C=C, random_state=42)
        svm_c.fit(X_train_c, y_train_c)
        
        train_pred = svm_c.predict(X_train_c)
        test_pred = svm_c.predict(X_test_c)
        
        train_acc = accuracy_score(y_train_c, train_pred)
        test_acc = accuracy_score(y_test_c, test_pred)
        train_errors = np.sum(train_pred != y_train_c)
        test_errors = np.sum(test_pred != y_test_c)
        
        results.append({
            'C': C,
            'svm': svm_c,
            'train_acc': train_acc,
            'test_acc': test_acc,
            'train_errors': train_errors,
            'test_errors': test_errors,
            'sv_count': len(svm_c.support_vectors_)
        })
        
        print(f"{C:10.3f} | {train_acc:10.4f} | {test_acc:10.4f} | {train_errors:12} | {test_errors:12} | {svm_c.support_vectors_.shape[0]:10}")
    
    # Находим C, при котором достигается нулевая ошибка на обучении
    zero_train = [r for r in results if r['train_errors'] == 0]
    
    if zero_train:
        print("\n--- Параметры C с нулевой ошибкой на обучении ---")
        for r in zero_train:
            print(f"C = {r['C']:.3f}: test_acc = {r['test_acc']:.4f}, SV = {r['sv_count']}")
        
        # Выбираем оптимальный C (максимальная точность на тесте среди нулевой ошибки на обучении)
        best = max(zero_train, key=lambda x: x['test_acc'])
        print(f"\n✓ Оптимальный C = {best['C']:.3f}")
        print(f"  - Точность на обучении: {best['train_acc']:.4f} (0 ошибок)")
        print(f"  - Точность на тесте: {best['test_acc']:.4f}")
        print(f"  - Количество опорных векторов: {best['sv_count']}")
        
        clf_c_best = best['svm']
    else:
        # Если нулевой ошибки нет, берем максимальную точность на тесте
        best = max(results, key=lambda x: x['test_acc'])
        print(f"\n⚠ Нулевая ошибка на обучении не достигнута")
        print(f"Лучший C = {best['C']:.3f} (по точности на тесте)")
        clf_c_best = best['svm']
    
    # Визуализация влияния параметра C
    fig, axes = plt.subplots(1, 2, figsize=(15, 5))
    
    # График точности от C
    axes[0].semilogx([r['C'] for r in results], [r['train_acc'] for r in results], 'o-', label='Train', linewidth=2)
    axes[0].semilogx([r['C'] for r in results], [r['test_acc'] for r in results], 's-', label='Test', linewidth=2)
    axes[0].axhline(y=1.0, color='g', linestyle='--', alpha=0.5, label='Zero error')
    axes[0].set_xlabel('C (log scale)')
    axes[0].set_ylabel('Accuracy')
    axes[0].set_title('Влияние параметра C на точность')
    axes[0].legend()
    axes[0].grid(True, alpha=0.3)
    
    # График количества опорных векторов
    axes[1].semilogx([r['C'] for r in results], [r['sv_count'] for r in results], 'o-', color='orange', linewidth=2)
    axes[1].set_xlabel('C (log scale)')
    axes[1].set_ylabel('Number of Support Vectors')
    axes[1].set_title('Количество опорных векторов от C')
    axes[1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('svm_C_optimization.png', dpi=150)
    plt.show()
    
    # Визуализация границы с оптимальным C
    fig, axes = plt.subplots(1, 2, figsize=(16, 7))
    plot_decision_boundary(X_train_c, y_train_c, clf_c_best, 
                          f"svmdata_c (обучение) - C={best['C']:.3f}", 
                          ax=axes[0], show_support_vectors=True)
    plot_decision_boundary(X_test_c, y_test_c, clf_c_best, 
                          f"svmdata_c (тест) - C={best['C']:.3f}", 
                          ax=axes[1], show_support_vectors=True)
    plt.tight_layout()
    plt.savefig('svm_c_best_boundary.png', dpi=150)
    plt.show()
    
    print("\n--- ВЫВОД по заданию 4c ---")
    print("1. При малых C граница решения простая (большой зазор), но возможны ошибки")
    print("2. При увеличении C модель стремится классифицировать все обучающие точки правильно")
    print("3. Слишком большое C может привести к переобучению и снижению точности на тесте")
    print("4. Не всегда нужно добиваться нулевой ошибки на обучении - важен баланс")
    print("5. Оптимальный C выбран по максимуму точности на тестовой выборке")
    
except FileNotFoundError:
    print("Файлы svmdata_c.txt или svmdata_c_test.txt не найдены!")
    print("Для демонстрации используем svmdata_b (как наиболее сложный)")
    X_train_c, y_train_c = X_train_b, y_train_b
    X_test_c, y_test_c = X_test_b, y_test_b
    
    # Аналогичный код для svmdata_b
    C_values = [0.001, 0.01, 0.1, 0.5, 1, 5, 10, 50, 100, 500, 1000]
    results = []
    
    for C in C_values:
        svm_c = svm.SVC(kernel='linear', C=C, random_state=42)
        svm_c.fit(X_train_c, y_train_c)
        train_acc = accuracy_score(y_train_c, svm_c.predict(X_train_c))
        test_acc = accuracy_score(y_test_c, svm_c.predict(X_test_c))
        results.append({'C': C, 'svm': svm_c, 'train_acc': train_acc, 'test_acc': test_acc,
                       'train_errors': np.sum(svm_c.predict(X_train_c) != y_train_c),
                       'test_errors': np.sum(svm_c.predict(X_test_c) != y_test_c),
                       'sv_count': len(svm_c.support_vectors_)})
    
    best = max(results, key=lambda x: x['test_acc'])
    clf_c_best = best['svm']
    print(f"\nОптимальный C для svmdata_b: {best['C']:.3f}")

# ============================================
# ЗАДАНИЕ 4d и 4e: Различные ядра
# ============================================
print("\n" + "="*60)
print("ЗАДАНИЯ 4d и 4e: Сравнение различных ядер")
print("="*60)

# Используем данные svmdata_b как наиболее интересные для демонстрации
X_demo, y_demo = X_train_b, y_train_b
X_test_demo, y_test_demo = X_test_b, y_test_b

# Определяем различные ядра
kernels = {
    'Linear (линейное)': {'kernel': 'linear', 'params': {}},
    'Poly degree 1': {'kernel': 'poly', 'params': {'degree': 1, 'coef0': 1}},
    'Poly degree 2': {'kernel': 'poly', 'params': {'degree': 2, 'coef0': 1}},
    'Poly degree 3': {'kernel': 'poly', 'params': {'degree': 3, 'coef0': 1}},
    'Poly degree 4': {'kernel': 'poly', 'params': {'degree': 4, 'coef0': 1}},
    'Poly degree 5': {'kernel': 'poly', 'params': {'degree': 5, 'coef0': 1}},
    'RBF (гауссово)': {'kernel': 'rbf', 'params': {'gamma': 'scale'}},
    'Sigmoid': {'kernel': 'sigmoid', 'params': {'gamma': 'scale', 'coef0': 0}}
}

C_fixed = 1.0  # Фиксированный C для сравнения ядер

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

# Скрываем лишние подграфики
for idx in range(len(kernels), len(axes)):
    axes[idx].axis('off')

plt.suptitle('Сравнение различных ядер SVM (обучающая выборка)', fontsize=16, y=1.02)
plt.tight_layout()
plt.savefig('svm_kernels_comparison.png', dpi=150, bbox_inches='tight')
plt.show()

# Выводы по ядрам
print("\n--- ВЫВОДЫ по заданиям 4d и 4e ---")
print("1. Линейное ядро: хорошо для линейно разделимых данных, быстрое обучение")
print("2. Полиномиальные ядра:")
print("   - Степень 1 эквивалентна линейному")
print("   - Степень 2-3 часто дают хороший баланс")
print("   - Высокие степени (4-5) могут переобучаться")
print("3. RBF (гауссово) ядро:")
print("   - Универсальное, может моделировать сложные границы")
print("   - Чувствительно к параметру gamma")
print("4. Сигмоидальное ядро: работает как нейронная сеть")
print("   - Результаты сильно зависят от параметров")

# ============================================
# ЗАДАНИЕ 4f: Эффект переобучения (изменение gamma)
# ============================================
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

# Дополнительная визуализация: сложная граница при большом gamma
print("\n--- Детальный анализ переобучения ---")

# Создаем искусственные данные для демонстрации переобучения
np.random.seed(42)
X_complex = np.random.randn(100, 2) * 1.5
y_complex = np.zeros(100)
# Создаем сложную границу: круг
for i in range(100):
    if X_complex[i, 0]**2 + X_complex[i, 1]**2 < 1:
        y_complex[i] = 1

# Добавляем шум
X_complex[:20] += np.random.randn(20, 2) * 0.5

# Обучаем с разными gamma
fig, axes = plt.subplots(2, 3, figsize=(18, 12))
axes = axes.flatten()

test_gammas = [0.01, 0.1, 1, 5, 20, 100]

for idx, gamma in enumerate(test_gammas):
    svm_test = svm.SVC(kernel='rbf', gamma=gamma, C=10, random_state=42)
    svm_test.fit(X_complex, y_complex)
    
    plot_decision_boundary(X_complex, y_complex, svm_test, 
                          f"gamma={gamma}\nSV={len(svm_test.support_vectors_)}", 
                          ax=axes[idx], show_support_vectors=True)

plt.suptitle('Демонстрация переобучения RBF ядра при увеличении gamma', fontsize=16, y=1.02)
plt.tight_layout()
plt.savefig('svm_overfitting_demo.png', dpi=150, bbox_inches='tight')
plt.show()

# Итоговые выводы
print("\n" + "="*60)
print("ОБЩИЕ ВЫВОДЫ ПО ЗАДАНИЮ 4")
print("="*60)

print("""
4c. Выбор параметра C:
    - Малое C → широкий зазор, возможны ошибки на обучении
    - Большое C → узкий зазор, стремление к нулю ошибок на обучении
    - Оптимальное C выбирается по максимуму точности на валидации/тесте
    - Нулевая ошибка на обучении НЕ ВСЕГДА хороша (может быть переобучение)

4d-4e. Сравнение ядер:
    - Линейное: быстрое, для линейно разделимых данных
    - Полиномиальное: степень 2-3 оптимальна, высшие степени переобучаются
    - RBF: универсальное, но требует подбора gamma
    - Сигмоидальное: нестабильно, зависит от параметров

4f. Эффект переобучения (RBF, gamma):
    - Малая gamma → гладкая граница, недообучение
    - Оптимальная gamma → хорошее обобщение
    - Большая gamma → каждый образец создает свой "колокол", сильное переобучение
    - При переобучении: train_acc → 1, test_acc падает, растет число SV
    
Практические рекомендации:
    1. Начинайте с линейного ядра
    2. Для нелинейных данных используйте RBF
    3. Всегда используйте валидацию для подбора параметров
    4. Следите за разницей train/test accuracy
""")
# NOTE: 4c
# Загрузка данных
# # === ЗАДАНИЕ 4c: Изменение штрафного параметра C ===
# print("\n" + "="*60)
# print("ЗАДАНИЕ 4c: Исследование влияния параметра C")
# print("="*60)
#
# # Для svmdata_b (где данные нелинейно разделимы)
# C_values = [0.01, 0.1, 1, 10, 100, 1000]
# train_scores = []
# test_scores = []
# support_vectors_counts = []
#
# print("\nАнализ для svmdata_b:")
# print("-" * 50)
#
# for C in C_values:
#     clf = svm.SVC(kernel='linear', C=C, random_state=42)
#     clf.fit(X_train_b, y_train_b)
#
#     train_acc = accuracy_score(y_train_b, clf.predict(X_train_b))
#     test_acc = accuracy_score(y_test_b, clf.predict(X_test_b))
#     n_sv = len(clf.support_vectors_)
#
#     train_scores.append(train_acc)
#     test_scores.append(test_acc)
#     support_vectors_counts.append(n_sv)
#
#     print(f"C = {C:6.2f}: Train acc = {train_acc:.4f}, Test acc = {test_acc:.4f}, "
#           f"SV = {n_sv:2d}")
#
# # Визуализация зависимости от C
# fig, axes = plt.subplots(1, 2, figsize=(14, 5))
#
# # График точности
# axes[0].semilogx(C_values, train_scores, 'bo-', label='Обучающая выборка', linewidth=2)
# axes[0].semilogx(C_values, test_scores, 'rs-', label='Тестовая выборка', linewidth=2)
# axes[0].set_xlabel('Параметр C')
# axes[0].set_ylabel('Точность')
# axes[0].set_title('Зависимость точности от параметра C')
# axes[0].grid(True, alpha=0.3)
# axes[0].legend()
# axes[0].set_ylim([0.5, 1.05])
#
# # График количества опорных векторов
# axes[1].semilogx(C_values, support_vectors_counts, 'go-', linewidth=2)
# axes[1].set_xlabel('Параметр C')
# axes[1].set_ylabel('Количество опорных векторов')
# axes[1].set_title('Зависимость количества опорных векторов от C')
# axes[1].grid(True, alpha=0.3)
#
# plt.tight_layout()
# plt.savefig('svm_C_analysis.png', dpi=150)
# plt.show()
#
# # Визуализация границ для разных C
# fig, axes = plt.subplots(2, 3, figsize=(18, 10))
# axes = axes.flatten()
#
# for i, C in enumerate(C_values):
#     clf = svm.SVC(kernel='linear', C=C, random_state=42)
#     clf.fit(X_train_b, y_train_b)
#
#     plot_decision_boundary(X_train_b, y_train_b, clf, 
#                           f'svmdata_b, C={C}', 
#                           ax=axes[i], show_support_vectors=True)
#
# plt.tight_layout()
# plt.savefig('svm_C_comparison.png', dpi=150)
# plt.show()
#
# # Оптимальное значение C
# best_idx = np.argmax(test_scores)
# optimal_C = C_values[best_idx]
# print(f"\nОптимальное значение C = {optimal_C} (максимальная точность на тесте: {test_scores[best_idx]:.4f})")
#
# # === ЗАДАНИЕ 4d: Различные ядра ===
# print("\n" + "="*60)
# print("ЗАДАНИЕ 4d: Сравнение различных ядер")
# print("="*60)
#
# # Определение ядер для сравнения
# kernels = [
#     ('linear', 'Линейное'),
#     ('poly', 'Полиномиальное (степень 2)'),
#     ('poly', 'Полиномиальное (степень 3)'),
#     ('poly', 'Полиномиальное (степень 4)'),
#     ('poly', 'Полиномиальное (степень 5)'),
#     ('sigmoid', 'Сигмоидальное'),
#     ('rbf', 'Гауссово (RBF)')
# ]
#
# kernel_params = [
#     {'kernel': 'linear', 'C': 1.0},
#     {'kernel': 'poly', 'degree': 2, 'C': 1.0, 'coef0': 1},
#     {'kernel': 'poly', 'degree': 3, 'C': 1.0, 'coef0': 1},
#     {'kernel': 'poly', 'degree': 4, 'C': 1.0, 'coef0': 1},
#     {'kernel': 'poly', 'degree': 5, 'C': 1.0, 'coef0': 1},
#     {'kernel': 'sigmoid', 'C': 1.0, 'coef0': 1},
#     {'kernel': 'rbf', 'C': 1.0, 'gamma': 'scale'}
# ]
#
# print("\nСравнение для svmdata_b:")
# print("-" * 70)
# print(f"{'Ядро':<30} {'Train Acc':<12} {'Test Acc':<12} {'SV Count':<10}")
# print("-" * 70)
#
# results = []
#
# for kernel_name, kernel_params_dict in zip([k[1] for k in kernels], kernel_params):
#     clf = svm.SVC(**kernel_params_dict, random_state=42)
#     clf.fit(X_train_b, y_train_b)
#
#     train_acc = accuracy_score(y_train_b, clf.predict(X_train_b))
#     test_acc = accuracy_score(y_test_b, clf.predict(X_test_b))
#     n_sv = len(clf.support_vectors_)
#
#     results.append({
#         'kernel': kernel_name,
#         'train_acc': train_acc,
#         'test_acc': test_acc,
#         'n_sv': n_sv,
#         'clf': clf
#     })
#
#     print(f"{kernel_name:<30} {train_acc:.4f}       {test_acc:.4f}       {n_sv:2d}")
#
# print("-" * 70)
#
# # Визуализация границ для разных ядер
# fig, axes = plt.subplots(2, 4, figsize={20, 10})
# axes = axes.flatten()
#
# for i, result in enumerate(results[:8]):  # максимум 8 графиков
#     plot_decision_boundary(X_train_b, y_train_b, result['clf'], 
#                           f"{result['kernel']}\nTrain: {result['train_acc']:.3f}, Test: {result['test_acc']:.3f}", 
#                           ax=axes[i], show_support_vectors=True)
#
# # Скрываем лишний подграфик
# for i in range(len(results), 8):
#     axes[i].axis('off')
#
# plt.tight_layout()
# plt.savefig('svm_kernel_comparison.png', dpi=150)
# plt.show()
#
# # === ЗАДАНИЕ 4e-f: Эффект переобучения (изменение gamma для RBF ядра) ===
# print("\n" + "="*60)
# print("ЗАДАНИЕ 4e-f: Эффект переобучения (изменение gamma для RBF ядра)")
# print("="*60)
#
# gamma_values = [0.1, 1, 10, 100, 1000, 10000]
# train_scores_gamma = []
# test_scores_gamma = []
# sv_counts_gamma = []
#
# print("\nАнализ для svmdata_b с RBF ядром:")
# print("-" * 60)
# print(f"{'Gamma':<10} {'Train Acc':<12} {'Test Acc':<12} {'SV Count':<10}")
# print("-" * 60)
#
# for gamma in gamma_values:
#     clf = svm.SVC(kernel='rbf', gamma=gamma, C=1.0, random_state=42)
#     clf.fit(X_train_b, y_train_b)
#
#     train_acc = accuracy_score(y_train_b, clf.predict(X_train_b))
#     test_acc = accuracy_score(y_test_b, clf.predict(X_test_b))
#     n_sv = len(clf.support_vectors_)
#
#     train_scores_gamma.append(train_acc)
#     test_scores_gamma.append(test_acc)
#     sv_counts_gamma.append(n_sv)
#
#     print(f"{gamma:<10} {train_acc:.4f}       {test_acc:.4f}       {n_sv:2d}")
#
# print("-" * 60)
#
# # Визуализация зависимости от gamma
# fig, axes = plt.subplots(1, 2, figsize=(14, 5))
#
# # График точности
# axes[0].semilogx(gamma_values, train_scores_gamma, 'bo-', label='Обучающая выборка', linewidth=2)
# axes[0].semilogx(gamma_values, test_scores_gamma, 'rs-', label='Тестовая выборка', linewidth=2)
# axes[0].set_xlabel('Параметр gamma')
# axes[0].set_ylabel('Точность')
# axes[0].set_title('Зависимость точности от gamma (RBF ядро)')
# axes[0].grid(True, alpha=0.3)
# axes[0].legend()
# axes[0].set_ylim([0.5, 1.05])
#
# # График количества опорных векторов
# axes[1].semilogx(gamma_values, sv_counts_gamma, 'go-', linewidth=2)
# axes[1].set_xlabel('Параметр gamma')
# axes[1].set_ylabel('Количество опорных векторов')
# axes[1].set_title('Зависимость количества опорных векторов от gamma')
# axes[1].grid(True, alpha=0.3)
#
# plt.tight_layout()
# plt.savefig('svm_gamma_analysis.png', dpi=150)
# plt.show()
#
# # Визуализация границ для разных gamma
# fig, axes = plt.subplots(2, 3, figsize=(18, 10))
# axes = axes.flatten()
#
# for i, gamma in enumerate(gamma_values):
#     clf = svm.SVC(kernel='rbf', gamma=gamma, C=1.0, random_state=42)
#     clf.fit(X_train_b, y_train_b)
#
#     plot_decision_boundary(X_train_b, y_train_b, clf, 
#                           f'RBF ядро, gamma={gamma}\nTrain: {train_scores_gamma[i]:.3f}, Test: {test_scores_gamma[i]:.3f}', 
#                           ax=axes[i], show_support_vectors=True)
#
# plt.tight_layout()
# plt.savefig('svm_gamma_comparison.png', dpi=150)
# plt.show()
#
# # Анализ переобучения
# print("\n" + "="*60)
# print("АНАЛИЗ ПЕРЕОБУЧЕНИЯ")
# print("="*60)
#
# # Находим точку, где начинается переобучение
# train_test_gap = np.array(train_scores_gamma) - np.array(test_scores_gamma)
# overfit_start_idx = np.argmax(train_test_gap > 0.1)  # разрыв больше 10%
#
# if overfit_start_idx > 0:
#     print(f"Переобучение начинает проявляться при gamma > {gamma_values[overfit_start_idx-1]}")
#     print(f"Оптимальное значение gamma: {gamma_values[np.argmax(test_scores_gamma)]} "
#           f"(точность на тесте: {max(test_scores_gamma):.4f})")
# else:
#     print("Явного переобучения не наблюдается в исследованном диапазоне")
#
# print("\nВыводы:")
# print("1. Для линейно разделимых данных (svmdata_a) линейное ядро работает отлично")
# print("2. Для нелинейно разделимых данных (svmdata_b) нелинейные ядра дают лучшие результаты")
# print("3. Параметр C контролирует ширину зазора и количество опорных векторов")
# print("4. Слишком большое значение gamma приводит к переобучению (RBF ядро)")
# print("5. Полиномиальные ядра высоких степеней также могут переобучаться")
#
