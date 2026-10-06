import keyboard
import time
from timestamp import get_timestamp

class KeyboardLogger:
  def __init__(self, logger):
    self.logger = logger

  def _callback(self, event):
    timestamp = get_timestamp()
    direction = "down" if event.event_type == keyboard.KEY_DOWN else "release"
    log_message = f"{timestamp} {direction} '{event.name}'"
    self.logger.info(log_message)

  def run(self):
    keyboard.hook(self._callback)
    
    try:
      while True:
        time.sleep(1)
    except KeyboardInterrupt:
      pass
