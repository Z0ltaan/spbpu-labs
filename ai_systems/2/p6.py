#!/usr/bin/env python3
import pandas as pd
import numpy as np
import statsmodels.api as sm
import matplotlib.pyplot as plt

try:
    data = pd.read_csv("JohnsonJohnson.csv")
except FileNotFoundError:
    print("Файл JohnsonJohnson.csv не найден.")
    exit()

data["Year"] = data["index"].apply(lambda x: int(x.split()[0]))
data["Quarter"] = data["index"].apply(lambda x: x.split()[1])

plt.figure(figsize=(10, 6))
plt.grid(True, linestyle="--", alpha=0.5)

quarters = ["Q1", "Q2", "Q3", "Q4"]
colors = {"Q1": "#1f77b4", "Q2": "#ff7f0e", "Q3": "#2ca02c", "Q4": "#d62728"}

for q in quarters:
    q_data = data[data["Quarter"] == q]
    plt.plot(
        q_data["Year"], q_data["value"], marker="o", label=q, color=colors[q], alpha=0.8
    )

plt.title(
    "Поквартальная прибыль Johnson & Johnson (1960–1980)",
    fontsize=13,
    fontweight="bold",
)
plt.xlabel("Год", fontsize=11)
plt.ylabel("Прибыль", fontsize=11)
plt.legend(frameon=True)
plt.tight_layout()
plt.savefig("jj_quarters.png", dpi=300)
plt.show()

print("АНАЛИЗ ИНДИВИДУАЛЬНЫХ КВАРТАЛОВ:")
print("-" * 75)
print(
    f"{'Квартал':<8} | {'Наклон (Динамика)':<18} | {'R²':<7} | {'Прогноз на 2016':<15}"
)
print("-" * 75)

slopes = {}
forecasts_2016 = {}

for q in quarters:
    q_data = data[data["Quarter"] == q]
    X = sm.add_constant(q_data["Year"])
    y = q_data["value"]

    model = sm.OLS(y, X).fit()
    slope = model.params["Year"]
    slopes[q] = slope

    pred_2016 = model.predict([1, 2016])[0]
    forecasts_2016[q] = pred_2016

    print(f"{q:<8} | {slope:<18.4f} | {model.rsquared:.4f} | {pred_2016:<15.2f}")

X_all = sm.add_constant(data["Year"])
y_all = data["value"]
model_all = sm.OLS(y_all, X_all).fit()
pred_all_2016 = model_all.predict([1, 2016])[0]

print("-" * 75)
print(
    f"{'Все (All)':<8} | {model_all.params['Year']:<18.4f} | {model_all.rsquared:.4f} | {pred_all_2016:<15.2f} (по общей модели)"
)
print("-" * 75)

max_q = max(slopes, key=slopes.get)
min_q = min(slopes, key=slopes.get)

print(
    f"\nНаибольшая динамика доходности: {max_q} (прирост {slopes[max_q]:.4f} в год)"
)
print(f"Наименьшая динамика доходности: {min_q} (прирост {slopes[min_q]:.4f} в год)")

avg_yearly_forecast = np.mean(list(forecasts_2016.values()))
print(f"\nПРОГНОЗ НА 2016 ГОД:")
print(
    f"Средний прогноз по году (из поквартальных моделей): {avg_yearly_forecast:.2f}"
)
print(f"Средний прогноз по году (по общей модели):          {pred_all_2016:.2f}")
