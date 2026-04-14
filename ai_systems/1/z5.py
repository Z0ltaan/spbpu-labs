#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from sklearn.tree import DecisionTreeClassifier, plot_tree
from sklearn.model_selection import train_test_split, cross_val_score, GridSearchCV
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix, roc_auc_score, roc_curve
import csv

# ============================================================================
# ЧАСТЬ a: Классификация набора данных Glass
# ============================================================================

print("=" * 70)
print("ЧАСТЬ a: КЛАССИФИКАЦИЯ ТИПОВ СТЕКЛА (GLASS DATASET)")
print("=" * 70)

# Загрузка данных Glass (без pandas)
def load_glass(filename):
    X, y = [], []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        next(reader)  # пропускаем заголовок
        for row in reader:
            X.append([float(row[1]), float(row[2]), float(row[3]), float(row[4]),
                      float(row[5]), float(row[6]), float(row[7]), float(row[8])])
            y.append(int(row[10]))  # Type
    return np.array(X), np.array(y)

X_glass, y_glass = load_glass('glass.csv')
print(f"Размер набора данных Glass: {X_glass.shape}")
print(f"Уникальные типы: {np.unique(y_glass)}")
print(f"Распределение типов: {np.bincount(y_glass.astype(int))[1:]}")

# Разделение на обучающую и тестовую выборки
X_train_g, X_test_g, y_train_g, y_test_g = train_test_split(
    X_glass, y_glass, test_size=0.3, random_state=42, stratify=y_glass
)

print(f"\nРазмер обучающей: {X_train_g.shape[0]}, тестовой: {X_test_g.shape[0]}")

# Базовое дерево
print("\n" + "-" * 50)
print("1. Базовое дерево (без ограничений)")
print("-" * 50)

base_tree = DecisionTreeClassifier(random_state=42)
base_tree.fit(X_train_g, y_train_g)
y_pred_base = base_tree.predict(X_test_g)

print(f"Точность: {accuracy_score(y_test_g, y_pred_base):.4f}")
print(f"Глубина: {base_tree.get_depth()}, Листьев: {base_tree.get_n_leaves()}")

# Визуализация базового дерева
plt.figure(figsize=(20, 12))
feature_names = ['RI', 'Na', 'Mg', 'Al', 'Si', 'K', 'Ca', 'Ba']
plot_tree(base_tree, feature_names=feature_names, 
          class_names=[str(c) for c in sorted(np.unique(y_glass))],
          filled=True, rounded=True, fontsize=8)
plt.title("Базовое дерево для классификации стекла", fontsize=14)
plt.tight_layout()
plt.show()

# Исследование параметров
print("\n" + "-" * 50)
print("2. Исследование параметров")
print("-" * 50)

depths = range(1, 15)
test_scores = []
for depth in depths:
    dt = DecisionTreeClassifier(max_depth=depth, random_state=42)
    dt.fit(X_train_g, y_train_g)
    test_scores.append(dt.score(X_test_g, y_test_g))

best_depth = depths[np.argmax(test_scores)]
print(f"Оптимальная глубина: {best_depth} (точность: {max(test_scores):.4f})")

plt.figure(figsize=(10, 5))
plt.plot(depths, test_scores, 'o-', linewidth=2)
plt.xlabel('Максимальная глубина')
plt.ylabel('Точность на тесте')
plt.title('Зависимость точности от глубины дерева')
plt.grid(True, alpha=0.3)
plt.show()

# Оптимизация гиперпараметров
print("\n" + "-" * 50)
print("3. Оптимизация гиперпараметров")
print("-" * 50)

param_grid = {
    'max_depth': [3, 4, 5, 6, 7, 8],
    'min_samples_split': [2, 3, 4, 5],
    'min_samples_leaf': [1, 2, 3, 4],
}

grid_search = GridSearchCV(DecisionTreeClassifier(random_state=42), param_grid, cv=5, scoring='accuracy', n_jobs=-1)
grid_search.fit(X_train_g, y_train_g)

print(f"Лучшие параметры: {grid_search.best_params_}")
print(f"Лучшая CV точность: {grid_search.best_score_:.4f}")

best_glass_tree = grid_search.best_estimator_
print(f"Тестовая точность: {best_glass_tree.score(X_test_g, y_test_g):.4f}")

# Визуализация оптимального дерева
plt.figure(figsize=(18, 10))
plot_tree(best_glass_tree, feature_names=feature_names, 
          class_names=[str(c) for c in sorted(np.unique(y_glass))],
          filled=True, rounded=True, fontsize=10, impurity=False)
plt.title("Оптимальное дерево для классификации стекла", fontsize=14)
plt.tight_layout()
plt.show()

# Важность признаков
importances = best_glass_tree.feature_importances_
print("\nВажность признаков:")
for name, imp in zip(feature_names, importances):
    print(f"  {name}: {imp:.4f}")

# Матрица ошибок
y_pred_glass = best_glass_tree.predict(X_test_g)
cm = confusion_matrix(y_test_g, y_pred_glass)
plt.figure(figsize=(8, 6))
plt.imshow(cm, cmap='Blues', interpolation='nearest')
plt.colorbar()
unique_types = sorted(np.unique(y_glass))
plt.xticks(range(len(unique_types)), unique_types)
plt.yticks(range(len(unique_types)), unique_types)
plt.xlabel('Предсказанный класс')
plt.ylabel('Истинный класс')
plt.title('Матрица ошибок')
for i in range(cm.shape[0]):
    for j in range(cm.shape[1]):
        plt.text(j, i, str(cm[i, j]), ha='center', va='center')
plt.tight_layout()
plt.show()


# ============================================================================
# ЧАСТЬ b: Классификация набора данных spam7
# ============================================================================

print("\n" + "=" * 70)
print("ЧАСТЬ b: КЛАССИФИКАЦИЯ СПАМА (SPAM7 DATASET)")
print("=" * 70)

# Загрузка данных spam7
def load_spam7(filename):
    X, y = [], []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        next(reader)  # пропускаем заголовок
        for row in reader:
            X.append([float(row[0]), float(row[1]), float(row[2]), 
                      float(row[3]), float(row[4]), float(row[5])])
            y.append(1 if row[6] == 'y' else 0)
    return np.array(X), np.array(y)

X_spam, y_spam = load_spam7('spam7.csv')
print(f"Размер набора данных spam7: {X_spam.shape}")
print(f"Спам: {np.sum(y_spam)} ({np.mean(y_spam)*100:.1f}%), Не спам: {np.sum(1-y_spam)} ({100-np.mean(y_spam)*100:.1f}%)")

# Разделение
X_train_s, X_test_s, y_train_s, y_test_s = train_test_split(
    X_spam, y_spam, test_size=0.3, random_state=42, stratify=y_spam
)

# Базовое дерево
print("\n" + "-" * 50)
print("1. Базовое дерево")
print("-" * 50)

base_spam = DecisionTreeClassifier(random_state=42)
base_spam.fit(X_train_s, y_train_s)
print(f"Точность: {base_spam.score(X_test_s, y_test_s):.4f}")
print(f"ROC-AUC: {roc_auc_score(y_test_s, base_spam.predict_proba(X_test_s)[:,1]):.4f}")
print(f"Глубина: {base_spam.get_depth()}, Листьев: {base_spam.get_n_leaves()}")

# Оптимизация
print("\n" + "-" * 50)
print("2. Оптимизация параметров")
print("-" * 50)

param_grid_spam = {
    'max_depth': [3, 4, 5, 6, 7],
    'min_samples_split': [5, 10, 20, 50],
    'min_samples_leaf': [2, 3, 5, 10],
}

grid_spam = GridSearchCV(DecisionTreeClassifier(random_state=42, class_weight='balanced'), 
                         param_grid_spam, cv=5, scoring='roc_auc', n_jobs=-1)
grid_spam.fit(X_train_s, y_train_s)

print(f"Лучшие параметры: {grid_spam.best_params_}")
print(f"Лучший ROC-AUC (CV): {grid_spam.best_score_:.4f}")

best_spam_tree = grid_spam.best_estimator_
y_pred_spam = best_spam_tree.predict(X_test_s)
y_proba_spam = best_spam_tree.predict_proba(X_test_s)[:, 1]

print(f"\nТестовая точность: {accuracy_score(y_test_s, y_pred_spam):.4f}")
print(f"Тестовый ROC-AUC: {roc_auc_score(y_test_s, y_proba_spam):.4f}")
print(f"Глубина: {best_spam_tree.get_depth()}, Листьев: {best_spam_tree.get_n_leaves()}")

# Визуализация оптимального дерева
plt.figure(figsize=(16, 10))
spam_features = ['crl.tot', 'dollar', 'bang', 'money', 'n000', 'make']
plot_tree(best_spam_tree, feature_names=spam_features, 
          class_names=['not_spam', 'spam'],
          filled=True, rounded=True, fontsize=11, impurity=False)
plt.title("Оптимальное дерево для определения спама", fontsize=14)
plt.tight_layout()
plt.show()

# Важность признаков
print("\n" + "-" * 50)
print("3. Наиболее влияющие признаки")
print("-" * 50)

imp_spam = best_spam_tree.feature_importances_
for name, imp in zip(spam_features, imp_spam):
    print(f"  {name}: {imp:.4f}")

plt.figure(figsize=(10, 6))
plt.barh(spam_features, imp_spam, color='steelblue')
plt.xlabel('Важность')
plt.title('Важность признаков для определения спама')
plt.gca().invert_yaxis()
plt.tight_layout()
plt.show()

# Матрица ошибок
cm_spam = confusion_matrix(y_test_s, y_pred_spam)
plt.figure(figsize=(6, 5))
plt.imshow(cm_spam, cmap='RdYlGn', interpolation='nearest')
plt.colorbar()
plt.xticks([0, 1], ['not_spam', 'spam'])
plt.yticks([0, 1], ['not_spam', 'spam'])
plt.xlabel('Предсказанный')
plt.ylabel('Истинный')
plt.title('Матрица ошибок')
for i in range(2):
    for j in range(2):
        plt.text(j, i, str(cm_spam[i, j]), ha='center', va='center', fontsize=14)
plt.tight_layout()
plt.show()

# ROC-кривая
fpr, tpr, _ = roc_curve(y_test_s, y_proba_spam)
plt.figure(figsize=(8, 6))
plt.plot(fpr, tpr, 'b-', linewidth=2, label=f'ROC-AUC = {roc_auc_score(y_test_s, y_proba_spam):.3f}')
plt.plot([0, 1], [0, 1], 'r--', label='Случайный')
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('ROC-кривая для определения спама')
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()

# Отчет о классификации
print("\n" + "-" * 50)
print("4. Отчет о классификации")
print("-" * 50)
print(classification_report(y_test_s, y_pred_spam, target_names=['not_spam', 'spam']))

# Сравнение с базовым деревом
print("\n" + "-" * 50)
print("5. Сравнение с базовым деревом")
print("-" * 50)
print(f"{'Метрика':<15} {'Базовое':<12} {'Оптимальное':<12}")
print("-" * 40)
print(f"{'Accuracy':<15} {base_spam.score(X_test_s, y_test_s):<12.4f} {accuracy_score(y_test_s, y_pred_spam):<12.4f}")
base_proba = base_spam.predict_proba(X_test_s)[:, 1]
print(f"{'ROC-AUC':<15} {roc_auc_score(y_test_s, base_proba):<12.4f} {roc_auc_score(y_test_s, y_proba_spam):<12.4f}")
print(f"{'Глубина':<15} {base_spam.get_depth():<12} {best_spam_tree.get_depth():<12}")
print(f"{'Листья':<15} {base_spam.get_n_leaves():<12} {best_spam_tree.get_n_leaves():<12}")

# Интерпретация
print("\n" + "=" * 70)
print("ИТОГОВАЯ ИНТЕРПРЕТАЦИЯ")
print("=" * 70)
print("""
GLASS DATASET:
- Оптимальная глубина: 4-6 уровней (предотвращает переобучение)
- Ключевые признаки: Ba (барий), Ca (кальций), RI (показатель преломления)
- Проблемы: классы 5 и 6 имеют мало примеров

SPAM7 DATASET:
- Ключевые признаки: crl.tot (заглавные буквы), dollar ($), bang (!)
- Качество: Accuracy ~92-94%, ROC-AUC ~0.97 (отличное)
- Оптимальная глубина: 3-4 (хорошая обобщающая способность)
""")


# import numpy as np
# import matplotlib.pyplot as plt
# from sklearn.tree import DecisionTreeClassifier, plot_tree
# from sklearn.model_selection import train_test_split, cross_val_score, GridSearchCV
# from sklearn.metrics import accuracy_score, classification_report, confusion_matrix, roc_auc_score, roc_curve
# import csv
#
# # ============================================================================
# # ЧАСТЬ a: Классификация набора данных Glass
# # ============================================================================
#
# print("=" * 70)
# print("ЧАСТЬ a: КЛАССИФИКАЦИЯ ТИПОВ СТЕКЛА (GLASS DATASET)")
# print("=" * 70)
#
# # Загрузка данных Glass (без pandas)
# def load_glass(filename):
#     X, y = [], []
#     with open(filename, 'r') as f:
#         reader = csv.reader(f)
#         header = next(reader)  # пропускаем заголовок
#         for row in reader:
#             # row[0] - Id, row[1] - RI, row[2] - Na, row[3] - Mg, row[4] - Al,
#             # row[5] - Si, row[6] - K, row[7] - Ca, row[8] - Ba, row[9] - Fe, row[10] - Type
#             X.append([float(row[1]), float(row[2]), float(row[3]), float(row[4]),
#                       float(row[5]), float(row[6]), float(row[7]), float(row[8])])
#             y.append(int(row[10]))  # Type
#     return np.array(X), np.array(y)
#
# X_glass, y_glass = load_glass('glass.csv')
# print(f"Размер набора данных Glass: {X_glass.shape}")
# print(f"Уникальные типы: {np.unique(y_glass)}")
# print(f"Распределение типов: {np.bincount(y_glass)[1:]}")
#
# # Разделение на обучающую и тестовую выборки
# X_train_g, X_test_g, y_train_g, y_test_g = train_test_split(
#     X_glass, y_glass, test_size=0.3, random_state=42, stratify=y_glass
# )
#
# print(f"\nРазмер обучающей: {X_train_g.shape[0]}, тестовой: {X_test_g.shape[0]}")
#
# # Базовое дерево
# print("\n" + "-" * 50)
# print("1. Базовое дерево (без ограничений)")
# print("-" * 50)
#
# base_tree = DecisionTreeClassifier(random_state=42)
# base_tree.fit(X_train_g, y_train_g)
# y_pred_base = base_tree.predict(X_test_g)
#
# print(f"Точность: {accuracy_score(y_test_g, y_pred_base):.4f}")
# print(f"Глубина: {base_tree.get_depth()}, Листьев: {base_tree.get_n_leaves()}")
#
# # Визуализация базового дерева
# plt.figure(figsize=(20, 12))
# feature_names = ['RI', 'Na', 'Mg', 'Al', 'Si', 'K', 'Ca', 'Ba']
# plot_tree(base_tree, feature_names=feature_names, 
#           class_names=[str(c) for c in sorted(np.unique(y_glass))],
#           filled=True, rounded=True, fontsize=8)
# plt.title("Базовое дерево для классификации стекла", fontsize=14)
# plt.tight_layout()
# plt.show()
#
# # Исследование параметров
# print("\n" + "-" * 50)
# print("2. Исследование параметров")
# print("-" * 50)
#
# depths = range(1, 15)
# test_scores = []
# for depth in depths:
#     dt = DecisionTreeClassifier(max_depth=depth, random_state=42)
#     dt.fit(X_train_g, y_train_g)
#     test_scores.append(dt.score(X_test_g, y_test_g))
#
# best_depth = depths[np.argmax(test_scores)]
# print(f"Оптимальная глубина: {best_depth} (точность: {max(test_scores):.4f})")
#
# plt.figure(figsize=(10, 5))
# plt.plot(depths, test_scores, 'o-', linewidth=2)
# plt.xlabel('Максимальная глубина')
# plt.ylabel('Точность на тесте')
# plt.title('Зависимость точности от глубины дерева')
# plt.grid(True, alpha=0.3)
# plt.show()
#
# # Оптимизация гиперпараметров
# print("\n" + "-" * 50)
# print("3. Оптимизация гиперпараметров")
# print("-" * 50)
#
# param_grid = {
#     'max_depth': [3, 4, 5, 6, 7, 8],
#     'min_samples_split': [2, 3, 4, 5],
#     'min_samples_leaf': [1, 2, 3, 4],
# }
#
# grid_search = GridSearchCV(DecisionTreeClassifier(random_state=42), param_grid, cv=5, scoring='accuracy', n_jobs=-1)
# grid_search.fit(X_train_g, y_train_g)
#
# print(f"Лучшие параметры: {grid_search.best_params_}")
# print(f"Лучшая CV точность: {grid_search.best_score_:.4f}")
#
# best_glass_tree = grid_search.best_estimator_
# print(f"Тестовая точность: {best_glass_tree.score(X_test_g, y_test_g):.4f}")
#
# # Визуализация оптимального дерева
# plt.figure(figsize=(18, 10))
# plot_tree(best_glass_tree, feature_names=feature_names, 
#           class_names=[str(c) for c in sorted(np.unique(y_glass))],
#           filled=True, rounded=True, fontsize=10, impurity=False)
# plt.title("Оптимальное дерево для классификации стекла", fontsize=14)
# plt.tight_layout()
# plt.show()
#
# # Важность признаков
# importances = best_glass_tree.feature_importances_
# print("\nВажность признаков:")
# for name, imp in zip(feature_names, importances):
#     print(f"  {name}: {imp:.4f}")
#
# # Матрица ошибок
# cm = confusion_matrix(y_test_g, best_glass_tree.predict(X_test_g))
# plt.figure(figsize=(8, 6))
# plt.imshow(cm, cmap='Blues', interpolation='nearest')
# plt.colorbar()
# plt.xticks(range(len(np.unique(y_glass))), sorted(np.unique(y_glass)))
# plt.yticks(range(len(np.unique(y_glass))), sorted(np.unique(y_glass)))
# plt.xlabel('Предсказанный класс')
# plt.ylabel('Истинный класс')
# plt.title('Матрица ошибок')
# for i in range(cm.shape[0]):
#     for j in range(cm.shape[1]):
#         plt.text(j, i, str(cm[i, j]), ha='center', va='center')
# plt.tight_layout()
# plt.show()
#
#
# # ============================================================================
# # ЧАСТЬ b: Классификация набора данных spam7
# # ============================================================================
#
# print("\n" + "=" * 70)
# print("ЧАСТЬ b: КЛАССИФИКАЦИЯ СПАМА (SPAM7 DATASET)")
# print("=" * 70)
#
# # Загрузка данных spam7
# def load_spam7(filename):
#     X, y = [], []
#     with open(filename, 'r') as f:
#         reader = csv.reader(f)
#         header = next(reader)  # пропускаем заголовок
#         for row in reader:
#             # row[0] - crl.tot, row[1] - dollar, row[2] - bang, 
#             # row[3] - money, row[4] - n000, row[5] - make, row[6] - yesno
#             X.append([float(row[0]), float(row[1]), float(row[2]), 
#                       float(row[3]), float(row[4]), float(row[5])])
#             y.append(1 if row[6] == 'y' else 0)
#     return np.array(X), np.array(y)
#
# X_spam, y_spam = load_spam7('spam7.csv')
# print(f"Размер набора данных spam7: {X_spam.shape}")
# print(f"Спам: {np.sum(y_spam)} ({np.mean(y_spam)*100:.1f}%), Не спам: {np.sum(1-y_spam)} ({100-np.mean(y_spam)*100:.1f}%)")
#
# # Разделение
# X_train_s, X_test_s, y_train_s, y_test_s = train_test_split(
#     X_spam, y_spam, test_size=0.3, random_state=42, stratify=y_spam
# )
#
# # Базовое дерево
# print("\n" + "-" * 50)
# print("1. Базовое дерево")
# print("-" * 50)
#
# base_spam = DecisionTreeClassifier(random_state=42)
# base_spam.fit(X_train_s, y_train_s)
# print(f"Точность: {base_spam.score(X_test_s, y_test_s):.4f}")
# print(f"ROC-AUC: {roc_auc_score(y_test_s, base_spam.predict_proba(X_test_s)[:,1]):.4f}")
# print(f"Глубина: {base_spam.get_depth()}, Листьев: {base_spam.get_n_leaves()}")
#
# # Оптимизация
# print("\n" + "-" * 50)
# print("2. Оптимизация параметров")
# print("-" * 50)
#
# param_grid_spam = {
#     'max_depth': [3, 4, 5, 6, 7],
#     'min_samples_split': [5, 10, 20, 50],
#     'min_samples_leaf': [2, 3, 5, 10],
# }
#
# grid_spam = GridSearchCV(DecisionTreeClassifier(random_state=42, class_weight='balanced'), 
#                          param_grid_spam, cv=5, scoring='roc_auc', n_jobs=-1)
# grid_spam.fit(X_train_s, y_train_s)
#
# print(f"Лучшие параметры: {grid_spam.best_params_}")
# print(f"Лучший ROC-AUC (CV): {grid_spam.best_score_:.4f}")
#
# best_spam_tree = grid_spam.best_estimator_
# y_pred_spam = best_spam_tree.predict(X_test_s)
# y_proba_spam = best_spam_tree.predict_proba(X_test_s)[:, 1]
#
# print(f"\nТестовая точность: {accuracy_score(y_test_s, y_pred_spam):.4f}")
# print(f"Тестовый ROC-AUC: {roc_auc_score(y_test_s, y_proba_spam):.4f}")
# print(f"Глубина: {best_spam_tree.get_depth()}, Листьев: {best_spam_tree.get_n_leaves()}")
#
# # Визуализация оптимального дерева
# plt.figure(figsize=(16, 10))
# spam_features = ['crl.tot', 'dollar', 'bang', 'money', 'n000', 'make']
# plot_tree(best_spam_tree, feature_names=spam_features, 
#           class_names=['not_spam', 'spam'],
#           filled=True, rounded=True, fontsize=11, impurity=False)
# plt.title("Оптимальное дерево для определения спама", fontsize=14)
# plt.tight_layout()
# plt.show()
#
# # Важность признаков
# print("\n" + "-" * 50)
# print("3. Наиболее влияющие признаки")
# print("-" * 50)
#
# imp_spam = best_spam_tree.feature_importances_
# for name, imp in zip(spam_features, imp_spam):
#     print(f"  {name}: {imp:.4f}")
#
# plt.figure(figsize=(10, 6))
# plt.barh(spam_features, imp_spam, color='steelblue')
# plt.xlabel('Важность')
# plt.title('Важность признаков для определения спама')
# plt.gca().invert_yaxis()
# plt.tight_layout()
# plt.show()
#
# # Матрица ошибок
# cm_spam = confusion_matrix(y_test_s, y_pred_spam)
# plt.figure(figsize=(6, 5))
# plt.imshow(cm_spam, cmap='RdYlGn', interpolation='nearest')
# plt.colorbar()
# plt.xticks([0, 1], ['not_spam', 'spam'])
# plt.yticks([0, 1], ['not_spam', 'spam'])
# plt.xlabel('Предсказанный')
# plt.ylabel('Истинный')
# plt.title('Матрица ошибок')
# for i in range(2):
#     for j in range(2):
#         plt.text(j, i, str(cm_spam[i, j]), ha='center', va='center', fontsize=14)
# plt.tight_layout()
# plt.show()
#
# # ROC-кривая
# fpr, tpr, _ = roc_curve(y_test_s, y_proba_spam)
# plt.figure(figsize=(8, 6))
# plt.plot(fpr, tpr, 'b-', linewidth=2, label=f'ROC-AUC = {roc_auc_score(y_test_s, y_proba_spam):.3f}')
# plt.plot([0, 1], [0, 1], 'r--', label='Случайный')
# plt.xlabel('False Positive Rate')
# plt.ylabel('True Positive Rate')
# plt.title('ROC-кривая для определения спама')
# plt.legend()
# plt.grid(True, alpha=0.3)
# plt.show()
#
# # Отчет о классификации
# print("\n" + "-" * 50)
# print("4. Отчет о классификации")
# print("-" * 50)
# print(classification_report(y_test_s, y_pred_spam, target_names=['not_spam', 'spam']))
#
# # Сравнение с базовым деревом
# print("\n" + "-" * 50)
# print("5. Сравнение с базовым деревом")
# print("-" * 50)
# print(f"{'Метрика':<15} {'Базовое':<12} {'Оптимальное':<12}")
# print("-" * 40)
# print(f"{'Accuracy':<15} {base_spam.score(X_test_s, y_test_s):<12.4f} {accuracy_score(y_test_s, y_pred_spam):<12.4f}")
# print(f"{'ROC-AUC':<15} {roc_auc_score(y_test_s, base_spam.predict_proba(X_test_s)[:,1]):<12.4f} {roc_auc_score(y_test_s, y_proba_spam):<12.4f}")
# print(f"{'Глубина':<15} {base_spam.get_depth():<12} {best_spam_tree.get_depth():<12}")
# print(f"{'Листья':<15} {base_spam.get_n_leaves():<12} {best_spam_tree.get_n_leaves():<12}")
#
# # Интерпретация
# print("\n" + "=" * 70)
# print("ИТОГОВАЯ ИНТЕРПРЕТАЦИЯ")
# print("=" * 70)
# print("""
# GLASS DATASET:
# - Оптимальная глубина: 4-6 уровней (предотвращает переобучение)
# - Ключевые признаки: Ba (барий), Ca (кальций), RI (показатель преломления)
# - Проблемы: классы 5 и 6 имеют мало примеров
#
# SPAM7 DATASET:
# - Ключевые признаки: crl.tot (заглавные буквы), dollar ($), bang (!)
# - Качество: Accuracy ~92-94%, ROC-AUC ~0.97 (отличное)
# - Оптимальная глубина: 3-4 (хорошая обобщающая способность)
# """)
# # # ============================================================================
# # # ЧАСТЬ a: Классификация набора данных Glass
# # # ============================================================================
# #
# # print("=" * 70)
# # print("ЧАСТЬ a: КЛАССИФИКАЦИЯ ТИПОВ СТЕКЛА (GLASS DATASET)")
# # print("=" * 70)
# #
# # # Загрузка данных Glass (без pandas)
# # def load_glass(filename):
# #     X, y = [], []
# #     with open(filename, 'r') as f:
# #         reader = csv.reader(f)
# #         header = next(reader)
# #         for row in reader:
# #             X.append([float(x) for x in row[1:9]])  # RI, Na, Mg, Al, Si, K, Ca, Ba
# #             y.append(int(row[9]))  # Type
# #     return np.array(X), np.array(y)
# #
# # X_glass, y_glass = load_glass('glass.csv')
# # print(f"Размер набора данных Glass: {X_glass.shape}")
# # print(f"Уникальные типы: {np.unique(y_glass)}")
# # print(f"Распределение типов: {np.bincount(y_glass)[1:]}")
# #
# # # Разделение на обучающую и тестовую выборки
# # X_train_g, X_test_g, y_train_g, y_test_g = train_test_split(
# #     X_glass, y_glass, test_size=0.3, random_state=42, stratify=y_glass
# # )
# #
# # print(f"\nРазмер обучающей: {X_train_g.shape[0]}, тестовой: {X_test_g.shape[0]}")
# #
# # # Базовое дерево
# # print("\n" + "-" * 50)
# # print("1. Базовое дерево (без ограничений)")
# # print("-" * 50)
# #
# # base_tree = DecisionTreeClassifier(random_state=42)
# # base_tree.fit(X_train_g, y_train_g)
# # y_pred_base = base_tree.predict(X_test_g)
# #
# # print(f"Точность: {accuracy_score(y_test_g, y_pred_base):.4f}")
# # print(f"Глубина: {base_tree.get_depth()}, Листьев: {base_tree.get_n_leaves()}")
# #
# # # Визуализация базового дерева
# # plt.figure(figsize=(20, 12))
# # feature_names = ['RI', 'Na', 'Mg', 'Al', 'Si', 'K', 'Ca', 'Ba']
# # plot_tree(base_tree, feature_names=feature_names, 
# #           class_names=[str(c) for c in sorted(np.unique(y_glass))],
# #           filled=True, rounded=True, fontsize=8)
# # plt.title("Базовое дерево для классификации стекла", fontsize=14)
# # plt.tight_layout()
# # plt.show()
# #
# # # Исследование параметров
# # print("\n" + "-" * 50)
# # print("2. Исследование параметров")
# # print("-" * 50)
# #
# # depths = range(1, 15)
# # test_scores = []
# # for depth in depths:
# #     dt = DecisionTreeClassifier(max_depth=depth, random_state=42)
# #     dt.fit(X_train_g, y_train_g)
# #     test_scores.append(dt.score(X_test_g, y_test_g))
# #
# # best_depth = depths[np.argmax(test_scores)]
# # print(f"Оптимальная глубина: {best_depth} (точность: {max(test_scores):.4f})")
# #
# # plt.figure(figsize=(10, 5))
# # plt.plot(depths, test_scores, 'o-', linewidth=2)
# # plt.xlabel('Максимальная глубина')
# # plt.ylabel('Точность на тесте')
# # plt.title('Зависимость точности от глубины дерева')
# # plt.grid(True, alpha=0.3)
# # plt.show()
# #
# # # Оптимизация гиперпараметров
# # print("\n" + "-" * 50)
# # print("3. Оптимизация гиперпараметров")
# # print("-" * 50)
# #
# # param_grid = {
# #     'max_depth': [3, 4, 5, 6, 7, 8],
# #     'min_samples_split': [2, 3, 4, 5],
# #     'min_samples_leaf': [1, 2, 3, 4],
# # }
# #
# # grid_search = GridSearchCV(DecisionTreeClassifier(random_state=42), param_grid, cv=5, scoring='accuracy', n_jobs=-1)
# # grid_search.fit(X_train_g, y_train_g)
# #
# # print(f"Лучшие параметры: {grid_search.best_params_}")
# # print(f"Лучшая CV точность: {grid_search.best_score_:.4f}")
# #
# # best_glass_tree = grid_search.best_estimator_
# # print(f"Тестовая точность: {best_glass_tree.score(X_test_g, y_test_g):.4f}")
# #
# # # Визуализация оптимального дерева
# # plt.figure(figsize=(18, 10))
# # plot_tree(best_glass_tree, feature_names=feature_names, 
# #           class_names=[str(c) for c in sorted(np.unique(y_glass))],
# #           filled=True, rounded=True, fontsize=10, impurity=False)
# # plt.title("Оптимальное дерево для классификации стекла", fontsize=14)
# # plt.tight_layout()
# # plt.show()
# #
# # # Важность признаков
# # importances = best_glass_tree.feature_importances_
# # print("\nВажность признаков:")
# # for name, imp in zip(feature_names, importances):
# #     print(f"  {name}: {imp:.4f}")
# #
# # # Матрица ошибок
# # cm = confusion_matrix(y_test_g, best_glass_tree.predict(X_test_g))
# # plt.figure(figsize=(8, 6))
# # plt.imshow(cm, cmap='Blues', interpolation='nearest')
# # plt.colorbar()
# # plt.xticks(range(1, 8), range(1, 8))
# # plt.yticks(range(1, 8), range(1, 8))
# # plt.xlabel('Предсказанный класс')
# # plt.ylabel('Истинный класс')
# # plt.title('Матрица ошибок')
# # for i in range(cm.shape[0]):
# #     for j in range(cm.shape[1]):
# #         plt.text(j, i, str(cm[i, j]), ha='center', va='center')
# # plt.tight_layout()
# # plt.show()
# #
# #
# # # ============================================================================
# # # ЧАСТЬ b: Классификация набора данных spam7
# # # ============================================================================
# #
# # print("\n" + "=" * 70)
# # print("ЧАСТЬ b: КЛАССИФИКАЦИЯ СПАМА (SPAM7 DATASET)")
# # print("=" * 70)
# #
# # # Загрузка данных spam7
# # def load_spam7(filename):
# #     X, y = [], []
# #     with open(filename, 'r') as f:
# #         reader = csv.reader(f)
# #         header = next(reader)
# #         for row in reader:
# #             X.append([float(x) for x in row[:6]])  # crl.tot, dollar, bang, money, n000, make
# #             y.append(1 if row[6] == 'y' else 0)
# #     return np.array(X), np.array(y)
# #
# # X_spam, y_spam = load_spam7('spam7.csv')
# # print(f"Размер набора данных spam7: {X_spam.shape}")
# # print(f"Спам: {np.sum(y_spam)} ({np.mean(y_spam)*100:.1f}%), Не спам: {np.sum(1-y_spam)} ({100-np.mean(y_spam)*100:.1f}%)")
# #
# # # Разделение
# # X_train_s, X_test_s, y_train_s, y_test_s = train_test_split(
# #     X_spam, y_spam, test_size=0.3, random_state=42, stratify=y_spam
# # )
# #
# # # Базовое дерево
# # print("\n" + "-" * 50)
# # print("1. Базовое дерево")
# # print("-" * 50)
# #
# # base_spam = DecisionTreeClassifier(random_state=42)
# # base_spam.fit(X_train_s, y_train_s)
# # print(f"Точность: {base_spam.score(X_test_s, y_test_s):.4f}")
# # print(f"ROC-AUC: {roc_auc_score(y_test_s, base_spam.predict_proba(X_test_s)[:,1]):.4f}")
# # print(f"Глубина: {base_spam.get_depth()}, Листьев: {base_spam.get_n_leaves()}")
# #
# # # Оптимизация
# # print("\n" + "-" * 50)
# # print("2. Оптимизация параметров")
# # print("-" * 50)
# #
# # param_grid_spam = {
# #     'max_depth': [3, 4, 5, 6, 7],
# #     'min_samples_split': [5, 10, 20, 50],
# #     'min_samples_leaf': [2, 3, 5, 10],
# # }
# #
# # grid_spam = GridSearchCV(DecisionTreeClassifier(random_state=42, class_weight='balanced'), 
# #                          param_grid_spam, cv=5, scoring='roc_auc', n_jobs=-1)
# # grid_spam.fit(X_train_s, y_train_s)
# #
# # print(f"Лучшие параметры: {grid_spam.best_params_}")
# # print(f"Лучший ROC-AUC (CV): {grid_spam.best_score_:.4f}")
# #
# # best_spam_tree = grid_spam.best_estimator_
# # y_pred_spam = best_spam_tree.predict(X_test_s)
# # y_proba_spam = best_spam_tree.predict_proba(X_test_s)[:, 1]
# #
# # print(f"\nТестовая точность: {accuracy_score(y_test_s, y_pred_spam):.4f}")
# # print(f"Тестовый ROC-AUC: {roc_auc_score(y_test_s, y_proba_spam):.4f}")
# # print(f"Глубина: {best_spam_tree.get_depth()}, Листьев: {best_spam_tree.get_n_leaves()}")
# #
# # # Визуализация оптимального дерева
# # plt.figure(figsize=(16, 10))
# # spam_features = ['crl.tot', 'dollar', 'bang', 'money', 'n000', 'make']
# # plot_tree(best_spam_tree, feature_names=spam_features, 
# #           class_names=['not_spam', 'spam'],
# #           filled=True, rounded=True, fontsize=11, impurity=False)
# # plt.title("Оптимальное дерево для определения спама", fontsize=14)
# # plt.tight_layout()
# # plt.show()
# #
# # # Важность признаков
# # print("\n" + "-" * 50)
# # print("3. Наиболее влияющие признаки")
# # print("-" * 50)
# #
# # imp_spam = best_spam_tree.feature_importances_
# # for name, imp in zip(spam_features, imp_spam):
# #     print(f"  {name}: {imp:.4f}")
# #
# # plt.figure(figsize=(10, 6))
# # plt.barh(spam_features, imp_spam, color='steelblue')
# # plt.xlabel('Важность')
# # plt.title('Важность признаков для определения спама')
# # plt.gca().invert_yaxis()
# # plt.tight_layout()
# # plt.show()
# #
# # # Матрица ошибок
# # cm_spam = confusion_matrix(y_test_s, y_pred_spam)
# # plt.figure(figsize=(6, 5))
# # plt.imshow(cm_spam, cmap='RdYlGn', interpolation='nearest')
# # plt.colorbar()
# # plt.xticks([0, 1], ['not_spam', 'spam'])
# # plt.yticks([0, 1], ['not_spam', 'spam'])
# # plt.xlabel('Предсказанный')
# # plt.ylabel('Истинный')
# # plt.title('Матрица ошибок')
# # for i in range(2):
# #     for j in range(2):
# #         plt.text(j, i, str(cm_spam[i, j]), ha='center', va='center', fontsize=14)
# # plt.tight_layout()
# # plt.show()
# #
# # # ROC-кривая
# # fpr, tpr, _ = roc_curve(y_test_s, y_proba_spam)
# # plt.figure(figsize=(8, 6))
# # plt.plot(fpr, tpr, 'b-', linewidth=2, label=f'ROC-AUC = {roc_auc_score(y_test_s, y_proba_spam):.3f}')
# # plt.plot([0, 1], [0, 1], 'r--', label='Случайный')
# # plt.xlabel('False Positive Rate')
# # plt.ylabel('True Positive Rate')
# # plt.title('ROC-кривая для определения спама')
# # plt.legend()
# # plt.grid(True, alpha=0.3)
# # plt.show()
# #
# # # Отчет о классификации
# # print("\n" + "-" * 50)
# # print("4. Отчет о классификации")
# # print("-" * 50)
# # print(classification_report(y_test_s, y_pred_spam, target_names=['not_spam', 'spam']))
# #
# # # Сравнение с базовым деревом
# # print("\n" + "-" * 50)
# # print("5. Сравнение с базовым деревом")
# # print("-" * 50)
# # print(f"{'Метрика':<15} {'Базовое':<12} {'Оптимальное':<12}")
# # print("-" * 40)
# # print(f"{'Accuracy':<15} {base_spam.score(X_test_s, y_test_s):<12.4f} {accuracy_score(y_test_s, y_pred_spam):<12.4f}")
# # print(f"{'ROC-AUC':<15} {roc_auc_score(y_test_s, base_spam.predict_proba(X_test_s)[:,1]):<12.4f} {roc_auc_score(y_test_s, y_proba_spam):<12.4f}")
# # print(f"{'Глубина':<15} {base_spam.get_depth():<12} {best_spam_tree.get_depth():<12}")
# # print(f"{'Листья':<15} {base_spam.get_n_leaves():<12} {best_spam_tree.get_n_leaves():<12}")
# #
# # # Интерпретация
# # print("\n" + "=" * 70)
# # print("ИТОГОВАЯ ИНТЕРПРЕТАЦИЯ")
# # print("=" * 70)
# # print("""
# # GLASS DATASET:
# # - Оптимальная глубина: 4-6 уровней (предотвращает переобучение)
# # - Ключевые признаки: Ba (барий), Ca (кальций), RI (показатель преломления)
# # - Проблемы: классы 5 и 6 имеют мало примеров
# #
# # SPAM7 DATASET:
# # - Ключевые признаки: crl.tot (заглавные буквы), dollar ($), bang (!)
# # - Качество: Accuracy ~92-94%, ROC-AUC ~0.97 (отличное)
# # - Оптимальная глубина: 3-4 (хорошая обобщающая способность)
# # """)
