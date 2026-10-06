import schemdraw
import schemdraw.elements as elm
import schemdraw.dsp as dsp

with schemdraw.Drawing(file='ofm_transmitter.png', dpi=300) as d:
    d.config(fontsize=12, unit=2)
    
    # --- ВЕРХНЯЯ ЧАСТЬ (МОДУЛЯТОР) ---
    # Генератор несущей ставим в координату (0, 4)
    osc = dsp.Oscillator().at((0, 4)).label('Генератор\n$\\sin(2\\pi F_0 t)$', 'left')
    d += osc
    
    # Перемножитель (модулятор) идет сразу вправо от генератора
    mixer = dsp.Mixer().right().at(osc.E).label('$\\times$', 'center')
    d += mixer
    
    # Выходной сигнал s(t)
    d += elm.Line().right().at(mixer.E).length(1.5).label('$s_{ОФМ}(t)$', 'right')
    
    # --- НИЖНЯЯ ЧАСТЬ (ДИФФЕРЕНЦИАЛЬНЫЙ КОДЕР) ---
    # Начинаем нижнюю линию строго под генератором на высоте y=0
    in_line = elm.Line().right().at((0, 0)).length(1.5).label('$a_k$\n(Биты)', 'left')
    d += in_line
    
    # Блок NOT
    not_gate = dsp.Box(w=1.2, h=0.8).right().at(in_line.end).label('NOT', 'center')
    d += not_gate
    
    # Сумматор по модулю 2 (XOR)
    xor_gate = dsp.Mixer().right().at(not_gate.E).label('$\\oplus$', 'center')
    d += xor_gate
    
    # Линия после XOR (сигнал d_k)
    line_dk = elm.Line().right().at(xor_gate.E).length(1.5)
    d += line_dk
    
    # Точка разветвления для обратной связи
    dot_fb = elm.Dot().at(line_dk.end)
    d += dot_fb
    
    # --- ПРЕОБРАЗОВАТЕЛЬ УРОВНЕЙ ---
    # Блок умножения на 2
    mul2 = dsp.Box(w=1.2, h=0.8).right().at(dot_fb.center).label('$\\times 2$')
    d += mul2
    
    # Блок вычитания 1
    sub1 = dsp.Mixer().right().at(mul2.E).label('$- 1$', 'center')
    d += sub1
    
    # Линия от преобразователя вверх к верхнему перемножителю (соединяем y=0 и y=4)
    d += elm.Line().up().at(sub1.E).to((sub1.E.x, mixer.S.y))
    d += elm.Line().to(mixer.S).label('$b_k$\n($\\pm 1$)', 'left')
    
    # --- ЦЕПЬ ОБРАТНОЙ СВЯЗИ (Линия задержки z^-1) ---
    # Спускаемся вниз от точки разветвления на уровень y=-2
    line_down = elm.Line().down().at(dot_fb.center).to((dot_fb.center.x, -2))
    d += line_down
    
    # Блок задержки z^-1 (рисуем влево)
    z1 = dsp.Box(w=1.2, h=0.8).left().at(line_down.end).label('$z^{-1}$')
    d += z1
    
    # Ведем линию обратной связи обратно к нижнему входу сумматора XOR
    d += elm.Line().left().at(z1.W).to((xor_gate.S.x, -2))
    d += elm.Line().up().to(xor_gate.S).label('$d_{k-1}$', 'right')

