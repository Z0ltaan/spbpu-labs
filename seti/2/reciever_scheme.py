import schemdraw
import schemdraw.elements as elm
import schemdraw.dsp as dsp

with schemdraw.Drawing(file='ofm_receiver.png', dpi=300) as d:
    d.config(fontsize=12, unit=2)
    
    # --- ВХОД И ОПОРНЫЙ ДЕМОДУЛЯТОР ---
    # Линия входа зашумленного ОФМ-сигнала
    in_line = elm.Line().right().at((0, 4)).length(1.5).label('$x(t)$\n(Вход)', 'left')
    d += in_line
    
    # Входной перемножитель (Смеситель)
    mixer = dsp.Mixer().right().at(in_line.end).label('$\\times$', 'center')
    d += mixer
    
    # Опорный генератор приемника (снизу от смесителя)
    osc = dsp.Oscillator().up().at((mixer.S.x, 1)).label('Опорный ген.\n$\\sin(2\\pi F_0 t)$', 'bottom')
    d += osc
    
    # Соединяем генератор со смесителем
    d += elm.Line().up().at(osc.N).to(mixer.S)
    
    # --- ОБРАБОТКА СИГНАЛА (Интегратор и Решающее устройство) ---
    # Линия от смесителя к интегратору
    d += elm.Line().right().at(mixer.E).length(1.2)
    
    # Блок интегрирования со сбросом
    integrator = dsp.Box(w=1.4, h=0.8).right().label('$\\int_0^{T_b}$', 'center')
    d += integrator
    
    # Линия к решающему устройству
    d += elm.Line().right().at(integrator.E).length(1.2)
    
    # Решающее устройство (Компаратор/Пороговый элемент)
    decision = dsp.Box(w=1.4, h=0.8).right().label('$> 0 ?$', 'center')
    d += decision
    
    # Линия после решающего устройства (относительные демодулированные символы)
    line_bk = elm.Line().right().at(decision.E).length(1.5)
    d += line_bk
    
    # Точка разветвления для дифференциального декодера
    dot_fb = elm.Dot().at(line_bk.end)
    d += dot_fb
    
    # --- ДИФФЕРЕНЦИАЛЬНЫЙ ДЕКОДЕР ---
    # Прямая ветка идет на верхний вход перемножителя знаков
    # Размещаем финальный перемножитель декодера в координатах (12, 4)
    dec_mixer = dsp.Mixer().right().at((11.5, 4)).label('$\\times$', 'center')
    d += dec_mixer
    
    # Соединяем точку разветвления с верхним входом перемножителя
    d += elm.Line().right().at(dot_fb.center).to((dot_fb.center.x + 1, dot_fb.center.y))
    d += elm.Line().up().to((dot_fb.center.x + 1, dec_mixer.N.y))
    d += elm.Line().right().to(dec_mixer.N).label('$\\hat{b}_k$', 'top')
    
    # Ветка задержки (спускаемся вниз на y=1.5)
    line_down = elm.Line().down().at(dot_fb.center).to((dot_fb.center.x, 1.5))
    d += line_down
    
    # Блок памяти на 1 бит (линия задержки z^-1)
    z1 = dsp.Box(w=1.2, h=0.8).right().at(line_down.end).label('$z^{-1}$')
    d += z1
    
    # Ведем задержанный сигнал к нижнему входу перемножителя знаков
    d += elm.Line().right().at(z1.E).to((dec_mixer.S.x, 1.5))
    d += elm.Line().up().to(dec_mixer.S).label('$\\hat{b}_{k-1}$', 'right')
    
    # Выход из финального перемножителя (восстановленные биты a_k)
    d += elm.Line().right().at(dec_mixer.E).length(1.5).label('$\\hat{a}_k$\n(Выход)', 'right')

