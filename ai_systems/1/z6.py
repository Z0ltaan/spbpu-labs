#!/usr/bin/env python3

import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.preprocessing import StandardScaler
from sklearn.impute import SimpleImputer
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import roc_auc_score, classification_report, confusion_matrix

# ==================== 1. ЗАГРУЗКА ДАННЫХ ====================
print("Загрузка данных...")
train_df = pd.read_csv('bank_scoring_train.csv')
test_df = pd.read_csv('bank_scoring_test.csv')

print(f"Обучающая выборка: {train_df.shape}")
print(f"Тестовая выборка: {test_df.shape}")
print("\nПервые 5 строк обучающей выборки:")
print(train_df.head())
print("\nИнформация о данных:")
print(train_df.info())

# ==================== 2. ПРЕДОБРАБОТКА ====================
# Разделяем признаки и целевую переменную
X_train = train_df.drop('SeriousDlqin2yrs', axis=1)
y_train = train_df['SeriousDlqin2yrs']
X_test = test_df.drop('SeriousDlqin2yrs', axis=1)
y_test = test_df['SeriousDlqin2yrs']

# Заполняем пропуски медианой
imputer = SimpleImputer(strategy='median')
X_train_imputed = imputer.fit_transform(X_train)
X_test_imputed = imputer.transform(X_test)

# Масштабируем признаки
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train_imputed)
X_test_scaled = scaler.transform(X_test_imputed)

print(f"\nПропуски в обучающей выборке после обработки: {np.isnan(X_train_scaled).sum()}")
print(f"Размерность признакового пространства: {X_train_scaled.shape[1]}")

# ==================== 3. СИСТЕМА 1: ЛОГИСТИЧЕСКАЯ РЕГРЕССИЯ ====================
print("\n" + "="*60)
print("СИСТЕМА 1: ЛОГИСТИЧЕСКАЯ РЕГРЕССИЯ")
print("="*60)

# Создаем и обучаем модель
lr_model = LogisticRegression(class_weight='balanced', random_state=42, max_iter=1000)
lr_model.fit(X_train_scaled, y_train)

# Прогнозируем вероятности и классы
y_pred_proba_lr = lr_model.predict_proba(X_test_scaled)[:, 1]
y_pred_lr = lr_model.predict(X_test_scaled)

# Оцениваем качество
roc_auc_lr = roc_auc_score(y_test, y_pred_proba_lr)
print(f"\nROC-AUC на тестовой выборке: {roc_auc_lr:.4f}")
print("\nClassification Report:")
print(classification_report(y_test, y_pred_lr))
print("Confusion Matrix:")
print(confusion_matrix(y_test, y_pred_lr))

# ==================== 4. СИСТЕМА 2: RANDOM FOREST ====================
print("\n" + "="*60)
print("СИСТЕМА 2: RANDOM FOREST")
print("="*60)

# Создаем и обучаем модель
rf_model = RandomForestClassifier(n_estimators=100, class_weight='balanced', random_state=42, n_jobs=-1)
rf_model.fit(X_train_scaled, y_train)

# Прогнозируем вероятности и классы
y_pred_proba_rf = rf_model.predict_proba(X_test_scaled)[:, 1]
y_pred_rf = rf_model.predict(X_test_scaled)

# Оцениваем качество
roc_auc_rf = roc_auc_score(y_test, y_pred_proba_rf)
print(f"\nROC-AUC на тестовой выборке: {roc_auc_rf:.4f}")
print("\nClassification Report:")
print(classification_report(y_test, y_pred_rf))
print("Confusion Matrix:")
print(confusion_matrix(y_test, y_pred_rf))

# ==================== 5. СРАВНЕНИЕ СИСТЕМ ====================
print("\n" + "="*60)
print("СРАВНЕНИЕ СИСТЕМ")
print("="*60)

print(f"\nЛогистическая регрессия ROC-AUC: {roc_auc_lr:.4f}")
print(f"Random Forest ROC-AUC: {roc_auc_rf:.4f}")
print(f"Разница: {abs(roc_auc_lr - roc_auc_rf):.4f}")

if roc_auc_rf > roc_auc_lr:
    print("\n✅ ЛУЧШЕ СРАБОТАЛА СИСТЕМА НА ОСНОВЕ RANDOM FOREST")
    print(f"   Преимущество: +{(roc_auc_rf - roc_auc_lr)*100:.2f}% по ROC-AUC")
    best_model = rf_model
    best_auc = roc_auc_rf
else:
    print("\n✅ ЛУЧШЕ СРАБОТАЛА СИСТЕМА НА ОСНОВЕ ЛОГИСТИЧЕСКОЙ РЕГРЕССИИ")
    print(f"   Преимущество: +{(roc_auc_lr - roc_auc_rf)*100:.2f}% по ROC-AUC")
    best_model = lr_model
    best_auc = roc_auc_lr

# ==================== 6. ВЫВОДЫ И РЕКОМЕНДАЦИИ ====================
print("\n" + "="*60)
print("ВЫВОДЫ")
print("="*60)

print(f"""
1. Обе модели показывают {'хорошее' if best_auc > 0.75 else 'удовлетворительное'} качество разделения классов.
2. ROC-AUC = {best_auc:.4f} означает, что модель {'хорошо' if best_auc > 0.8 else 'удовлетворительно'} отличает дефолтных клиентов от надежных.
3. Для кредитного скоринга важнее минимизировать ложноположительные (отказ хорошему клиенту) 
   и ложноотрицательные (пропуск дефолта) срабатывания.
4. Рекомендуется дополнительно:
   - Настроить порог принятия решений (не обязательно 0.5)
   - Провести калибровку вероятностей
   - Проанализировать важность признаков
""")

# Дополнительно: анализ важности признаков для Random Forest (если он лучше)
if best_model == rf_model:
    feature_importance = pd.DataFrame({
        'feature': X_train.columns,
        'importance': rf_model.feature_importances_
    }).sort_values('importance', ascending=False)
    print("\nТоп-5 наиболее важных признаков (Random Forest):")
    print(feature_importance.head(5))
