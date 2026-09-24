import keyboard


class KeyboardLogger:
  def __init__(self, logger):
    self.logger = logger

  def run(self):
    while True:
      event = keyboard.read_event()
      direction = "down" if event.event_type == keyboard.KEY_DOWN else "release"
      log_message = f"{direction} '{event.name}'"
      self.logger.info(log_message)
