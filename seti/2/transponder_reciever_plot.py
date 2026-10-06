import numpy as np
import scipy.special as special
import matplotlib.pyplot as plt

# --- ИСХОДНЫЕ ДАННЫЕ (из методички) ---
Tb = 1/1200          # Длительность бита, с
F0 = 1800            # Частота несущей, Гц
Fs = 64/Tb           # Частота дискретизации (64 отсчёта на бит)
a = np.array([1,0,1,1,0,1,0,0]) # Входная последовательность

# =====================================================================
# ЧАСТЬ 1: ПЕРЕДАТЧИК ОФМ (Пункт 2 задания)
# =====================================================================
# Дифференциальный кодер (b0 = 1)
b = np.ones(len(a)+1, dtype=int)
for k in range(1, len(a)+1):
    b[k] = b[k-1] if a[k-1]==1 else -b[k-1]
b = b[1:]            # Убираем начальный элемент b0

# Формирование ОФМ-сигнала на синусах
t = np.arange(0, len(a)*Tb, 1/Fs)
s = np.zeros_like(t)
spb = int(Fs*Tb)     # Отсчетов на один бит (Samples per bit)

for k in range(len(a)):
    idx = slice(k*spb, (k+1)*spb)
    # ЗАМЕНЕНО НА СИНУС в соответствии с формулой (2.3) методички
    s[idx] = b[k] * np.sin(2 * np.pi * F0 * t[idx])

# =====================================================================
# ЧАСТЬ 2: КОГЕРЕНТНЫЙ ПРИЕМНИК ОФМ (Пункт 4 задания)
# =====================================================================
# 1. Опорный генератор и смеситель (умножение перед интегратором)
# Когерентный прием: опорный сигнал строго совпадает по фазе и форме (синус)
carrier_rec = np.sin(2 * np.pi * F0 * t)
mixed_signal = s * carrier_rec

# 2. Интегратор со сбросом и пороговый блок (Решающее устройство)
integrator_signal = np.zeros_like(t)
received_b = np.zeros(len(a), dtype=int)

for k in range(len(a)):
    idx = slice(k * spb, (k + 1) * spb)
    
    # Моделируем кумулятивное накопление интеграла внутри каждого такта Tb
    # Деление на Fs эквивалентно умножению на шаг времени dt
    integrator_signal[idx] = np.cumsum(mixed_signal[idx]) / Fs
    
    # Считываем значение интеграла на границе такта (потенциал в конце интервала анализа)
    final_integral_val = integrator_signal[idx][-1]
    
    # Пороговый блок выносит решение о знаке относительного символа (> 0 ?)
    received_b[k] = 1 if final_integral_val > 0 else -1

# 3. Дифференциальный декодер
# Память приемника z^-1 инициализируется начальным b0 = 1
z_minus_1_rx = 1
decoded_a = np.zeros(len(a), dtype=int)

for k in range(len(a)):
    current_bk = received_b[k]
    
    # Перемножитель знаков: текущее решение сравнивается с предыдущим
    product = current_bk * z_minus_1_rx
    
    # Если знаки совпали (+1) -> смены фазы не было, это бит 1
    # Если знаки разные (-1) -> фаза перевернулась на 180°, это бит 0
    decoded_a[k] = 1 if product > 0 else 0
    
    # Перезапись в блок памяти z^-1 для следующего шага
    z_minus_1_rx = current_bk

# =====================================================================
# ЧАСТЬ 3: ОТРИСОВКА ВРЕМЕННЫХ ДИАГРАММ ПРИЕМНИКА
# =====================================================================
fig_rx, ax_rx = plt.subplots(4, 1, figsize=(10, 8), sharex=True)

# График 1: Входной сигнал приемника (наш s(t) на синусах)
ax_rx[0].plot(t, s, color='red', lw=1.2)
ax_rx[0].set_ylabel('$x(t)$\n(Входной ОФМ)')
ax_rx[0].set_title('Временные диаграммы работы когерентного приемника ОФМ (на синусах)')
ax_rx[0].grid(True)

# График 2: Выход перемножителя (смесителя)
ax_rx[1].plot(t, mixed_signal, color='purple', lw=1)
ax_rx[1].set_ylabel('$Y(t)$\n(Выход смесителя)')
ax_rx[1].grid(True)

# График 3: Выход интегратора со сбросом в конце каждого такта
ax_rx[2].plot(t, integrator_signal, color='brown', lw=1.5)
ax_rx[2].set_ylabel('Выход\nинтегратора')
ax_rx[2].grid(True)

# График 4: Итоговые восстановленные биты на выходе декодера
ax_rx[3].step(t, np.repeat(decoded_a, spb), where='post', color='blue', lw=2)
ax_rx[3].set_ylabel('$\\hat{a}_k$\n(Выходные биты)')
ax_rx[3].set_xlabel('Время t, с')
ax_rx[3].grid(True)
ax_rx[3].set_ylim(-0.2, 1.2)

# Вертикальные маркеры границ тактов (каждые Tb секунд)
for axi in ax_rx:
    for k in range(len(a) + 1):
        axi.axvline(x=k * Tb, color='gray', linestyle='--', alpha=0.5)

plt.tight_layout()
plt.show()

