#!/usr/bin/env python3
import os
import pandas as pd
import matplotlib.pyplot as plt

file_name = "nn_1.csv"

if os.path.exists(file_name):
    df = pd.read_csv(file_name, sep=None, engine="python")
    df.columns = df.columns.str.strip()
    print(f"Успешно загружен файл: {file_name}")
else:
    print(f"Ошибка: Файл {file_name} не найден в текущей директории!")
    exit()

x1_col = "x1" if "x1" in df.columns else df.columns[0]
x2_col = "x2" if "x2" in df.columns else df.columns[1]
class_col = "class" if "class" in df.columns else df.columns[-1]

class_1 = df[df[class_col] == 1]
class_minus_1 = df[df[class_col] == -1]

plt.figure(figsize=(8, 7))

plt.scatter(
    class_1[x1_col],
    class_1[x2_col],
    color="#FF4136",
    label="Класс 1 (Внутренний)",
    edgecolors="black",
    s=45,
    alpha=0.85,
)

plt.scatter(
    class_minus_1[x1_col],
    class_minus_1[x2_col],
    color="#0074D9",
    label="Класс -1 (Внешний)",
    edgecolors="black",
    s=45,
    alpha=0.85,
)

plt.xlabel(f"Признак {x1_col}", fontsize=11)
plt.ylabel(f"Признак {x2_col}", fontsize=11)
plt.title(
    f"Визуализация исходной структуры данных {file_name}",
    fontsize=12,
    fontweight="bold",
)
plt.legend(loc="upper right", frameon=True, shadow=True)
plt.grid(True, linestyle="--", alpha=0.5)
plt.axis("equal")

plt.show()
