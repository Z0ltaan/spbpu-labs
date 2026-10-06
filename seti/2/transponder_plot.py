import numpy as np
import matplotlib.pyplot as plt

# --- ИСХОДНЫЕ ДАННЫЕ ИЗ ТЗ ---
Fbit = 1200          # Скорость передачи, бит/с
Tb = 1 / Fbit        # Длительность бита, с (~0.000833 с)
F0 = 1800            # Частота несущей, Гц
Fs = 64 / Tb         # Частота дискретизации (64 отсчёта на бит)

a = np.array([1, 0, 1, 1, 0, 1, 0, 0])

# --- ДИФФЕРЕНЦИАЛЬНЫЙ КОДЕР ---
b = np.ones(len(a) + 1, dtype=int)
for k in range(1, len(a) + 1):
    b[k] = b[k - 1] if a[k - 1] == 1 else -b[k - 1]
b = b[1:]            # Убираем начальный опорный элемент b0

# --- ФОРМИРОВАНИЕ СИГНАЛА НА СИНУСАХ ---
t = np.arange(0, len(a) * Tb, 1 / Fs)
s = np.zeros_like(t)
spb = int(Fs * Tb)   # Количество отсчетов на один бит (Samples Per Bit)

for k in range(len(a)):
    idx = slice(k * spb, (k + 1) * spb)
    # Использование синуса в соответствии с ТЗ и формулой (2.3) методички
    s[idx] = b[k] * np.sin(2 * np.pi * F0 * t[idx])

# --- ПОСТРОЕНИЕ ВРЕМЕННЫХ ДИАГРАММ ---
fig, ax = plt.subplots(3, 1, figsize=(10, 7), sharex=True)

# 1. График входных бит a_k
ax[0].step(t, np.repeat(a, spb), where='post', color='blue', lw=2)
ax[0].set_ylabel('$a_k$\n(Входные биты)')
ax[0].set_title('Временные диаграммы работы передатчика ОФМ (на синусах)')
ax[0].grid(True)

# 2. График относительных символов b_k после кодера
ax[1].step(t, np.repeat(b, spb), where='post', color='green', lw=2)
ax[1].set_ylabel('$b_k$\n(Символы $\\pm 1$)')
ax[1].grid(True)
ax[1].set_ylim(-1.5, 1.5)

# 3. График выходного ОФМ-сигнала s(t)
ax[2].plot(t, s, color='red', lw=1.5)
ax[2].set_ylabel('$s(t)$\n(Выходной сигнал)')
ax[2].set_xlabel('Время t, с')
ax[2].grid(True)

# Добавление вертикальных линий границ бит для наглядности стыков фаз
for axi in ax:
    for k in range(len(a) + 1):
        axi.axvline(x=k * Tb, color='gray', linestyle='--', alpha=0.5)

plt.tight_layout()
plt.show()

