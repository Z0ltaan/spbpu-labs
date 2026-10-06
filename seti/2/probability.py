import numpy as np
import scipy.special as special
import matplotlib.pyplot as plt

# 1. Задаем диапазон отношения сигнал/шум Eb/N0 в дБ и переводим в разы
ebno_db = np.linspace(0, 18, 1000)
ebno_linear = 10**(ebno_db / 10)

# Функция Q(x) через erfc
def Q(x):
    return 0.5 * special.erfc(x / np.sqrt(2))

# 2. Расчет вероятностей ошибок по формулам из методички
Pe_AM  = Q(np.sqrt(ebno_linear / 2))
Pe_CHM = Q(np.sqrt(ebno_linear))
Pe_FM  = Q(np.sqrt(2 * ebno_linear))
Pe_OFM = 2 * Pe_FM * (1 - Pe_FM)  # ОФМ с когерентным приемом

# 3. Функция для точного поиска требуемого SNR (в дБ) для заданной вероятности ошибки
def get_required_snr(pe_array, target_pe):
    idx = np.argmin(np.abs(pe_array - target_pe))
    return ebno_db[idx]

# Расчет выигрышей для целевых точек 10^-4 и 10^-6
for target_error in [1e-4, 1e-6]:
    snr_am  = get_required_snr(Pe_AM, target_error)
    snr_chm = get_required_snr(Pe_CHM, target_error)
    snr_ofm = get_required_snr(Pe_OFM, target_error)
    
    print(f"--- Для вероятности ошибки Pe = {target_error} ---")
    print(f"  Требуемый SNR: АМ = {snr_am:.2f} дБ, ЧМ = {snr_chm:.2f} дБ, ОФМ = {snr_ofm:.2f} дБ")
    print(f"  Выигрыш ОФМ по сравнению с АМ: {snr_am - snr_ofm:.2f} дБ")
    print(f"  Выигрыш ОФМ по сравнению с ЧМ: {snr_chm - snr_ofm:.2f} дБ\n")

# 4. Построение графиков (Пункт 5 задания)
plt.figure(figsize=(9, 7))
plt.semilogy(ebno_db, Pe_AM,  label='АМ (Поэлементный когерентный прием)', linestyle='--', color='orange')
plt.semilogy(ebno_db, Pe_CHM, label='ЧМ (Поэлементный когерентный прием)', linestyle='-.', color='green')
plt.semilogy(ebno_db, Pe_OFM, label='ОФМ (Когерентный прием с диф. декодером)', linewidth=2, color='blue')

# Оформление сетки и осей
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.xlabel('Отношение сигнал/шум $E_b/N_0$, дБ', fontsize=11)
plt.ylabel('Вероятность ошибки $P_e$', fontsize=11)
plt.title('Кривые помехоустойчивости систем передачи данных (АБГШ канал)', fontsize=12)
plt.ylim(1e-7, 0.5)
plt.xlim(0, 18)
plt.legend(fontsize=10)

# Выделение целевого диапазона 10^-4 – 10^-6 горизонтальными линиями
plt.axhline(y=1e-4, color='red', linestyle=':', alpha=0.7, label='Граница диапазона $10^{-4}$')
plt.axhline(y=1e-6, color='red', linestyle=':', alpha=0.7, label='Граница диапазона $10^{-6}$')

plt.tight_layout()
plt.show()

