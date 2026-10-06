import datetime

def get_timestamp_format():
  return "%d-%m-%Y %H-%M-%S"

def get_timestamp():
  return datetime.datetime.now().strftime(get_timestamp_format())
