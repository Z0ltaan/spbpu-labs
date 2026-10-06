from pynput import keyboard
import time

class KeyboardLogger:
    def __init__(self, logger):
        self.logger = logger
        self.listener = None

    def on_press(self, key):
        # Format the key name neatly
        try:
            key_name = key.char
        except AttributeError:
            key_name = str(key).replace("Key.", "")
        
        self.logger.info(f"down '{key_name}'")

    def on_release(self, key):
        try:
            key_name = key.char
        except AttributeError:
            key_name = str(key).replace("Key.", "")
            
        self.logger.info(f"release '{key_name}'")

    def run(self):
        # Start the non-blocking listener
        self.listener = keyboard.Listener(
            on_press=self.on_press, 
            on_release=self.on_release
        )
        self.listener.start()
        
        # Keep the main thread alive
        while self.listener.running:
            time.sleep(0.1)
